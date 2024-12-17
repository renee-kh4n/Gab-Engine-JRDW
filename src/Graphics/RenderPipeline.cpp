#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "RenderPipeline.h"

using namespace gbe;

gbe::RenderPipeline::RenderPipeline(void* (*procaddressfunc)(const char*), Vector2 dimensions)
{
    this->resolution = dimensions;

    auto thing = procaddressfunc("glGetString");

    if (gladLoadGLLoader(procaddressfunc) == 0)
        throw "ERROR: glad failed to initialize.";

    this->postprocess = nullptr;

    //Framebuffers setup
    glViewport(0, 0, dimensions.x, dimensions.y);
    mFrameBuffer = new Framebuffer(dimensions);
    mDepthFrameBuffer = new Framebuffer(dimensions);

    //Shaders
    this->depthShader = new Shader("DefaultAssets/Shaders/simple.vert", "DefaultAssets/Shaders/depth.frag");
    this->uiShader = new Shader("DefaultAssets/Shaders/gui.vert", "DefaultAssets/Shaders/gui.frag");

    //Mesh
    this->default_quad = new Mesh("DefaultAssets/3D/plane.obj");
}

void RenderPipeline::SetPostProcessing(Shader* postprocess) {
    this->postprocess = postprocess;
}
bool RenderPipeline::TryPushLight(rendering::Light* data, bool priority) {

    if (this->lights_this_frame.size() == this->maxlights)
        return false;

    if (priority) {
        this->lights_this_frame.push_front(data);
        return true;
    }

    this->lights_this_frame.push_back(data);
    return true;
}

void gbe::RenderPipeline::RenderFrame(Vector3& from, Vector3& forward, Matrix4& _frustrum, float& nearclip, float& farclip)
{
#pragma region Rendering
    auto& lights_mframe = this->lights_this_frame;
    /* =============== Render here =============== */
    const auto render_scene_to_active_buffer = [=,&lights_mframe](Matrix4 tmat_PV, Shader* overrideShader = nullptr) {
        //Loop through all objects
        for (auto& obj : drawcalls)
        {
            int last_used_texture_slot = -1;
            const auto SelectEmptyTextureSlot = [&last_used_texture_slot]() {
                last_used_texture_slot++;
                glActiveTexture(GL_TEXTURE0 + last_used_texture_slot);
                return last_used_texture_slot;
            };

            auto curshader = obj->m_material->m_shader;

            //Enable chosen shader program
            if (overrideShader != nullptr)
                curshader = overrideShader;

            glUseProgram(curshader->shaderID);

            for (int i = 0; i < obj->m_material->textureOverrides.size(); i++)
            {
                auto& textureOverride = obj->m_material->textureOverrides[i];

                auto tex_slot = SelectEmptyTextureSlot();
                curshader->SetTextureOverride(textureOverride.parameterName, textureOverride.texture, tex_slot);
            }

            //Pass the necessary CPU-computed data to the object shader
            for (auto& call : obj->calls) {

                //check if shader has light array
                auto checkloc = glGetUniformLocation(curshader->shaderID, "lights[0].enabled");

                if (checkloc >= 0) {
                    size_t index = 0;
                    int group_index = 0;
                    for (auto lightdata : lights_mframe)
                    {
                        if (lightdata->changed == false)
                            continue;

                        auto index_fix = [&index]() {return "[" + std::to_string(index) + "]"; };

                        if (lightdata->GetType() == Light::DIRECTION) {
                            auto dirlight = (DirLight*)lightdata;

                            auto prev_split = 0.0f;

                            for (size_t i = 0; i < dirlight->shadowmaps.size(); i++)
                            {
                                auto next_split = 1.0f;

                                if (i < dirlight->cascade_splits.size())
                                    next_split = dirlight->cascade_splits[i];

                                prev_split = next_split;

                                curshader->SetOverride(("lights" + index_fix() + ".enabled").c_str(), true);
                                curshader->SetOverride(("lights" + index_fix() + ".group_id").c_str(), group_index);
                                curshader->SetOverride(("lights" + index_fix() + ".type").c_str(), (int)Light::DIRECTION);
                                curshader->SetOverride(("lights" + index_fix() + ".color").c_str(), (Vector3)(dirlight->color * dirlight->intensity));
                                curshader->SetOverride(("lights" + index_fix() + ".dir").c_str(), dirlight->dir);


                                curshader->SetOverride(("light_view_matrixes" + index_fix()).c_str(), dirlight->cascade_projections[i]);

                                auto tex_slot = SelectEmptyTextureSlot();
                                curshader->SetTextureOverride(("lightviews" + index_fix()).c_str(), dirlight->shadowmaps[i], tex_slot);

                                index++;
                            }
                        }

                        group_index++;
                    }
                    for (size_t cont = index; cont < 10; cont++)
                    {
                        std::string index_fix = "[" + std::to_string(cont) + "]";
                        curshader->SetOverride(("lights" + index_fix + ".enabled").c_str(), false);
                    }
                }

                //Transform data
                Matrix4 tmat = call.second;
                curshader->SetOverride("transform_model", tmat);
                curshader->SetOverride("transform_projection", tmat_PV);
                curshader->SetOverride("cameraPos", from);
                curshader->SetOverride("near_clip", nearclip);
                curshader->SetOverride("far_clip", farclip);

                for (auto& iterator : obj->m_material->overrides) {
                    switch (iterator.second.type)
                    {
                    case BOOL:
                        curshader->SetOverride(iterator.first, iterator.second.value_bool);
                        break;
                    case FLOAT:
                        curshader->SetOverride(iterator.first, iterator.second.value_float);
                        break;
                    case VEC2:
                        curshader->SetOverride(iterator.first, iterator.second.value_vec2);
                        break;
                    case VEC3:
                        curshader->SetOverride(iterator.first, iterator.second.value_vec3);
                        break;
                    case MAT4:
                        curshader->SetOverride(iterator.first, iterator.second.value_mat4);
                        break;
                    default:
                        break;
                    }
                }

                //Draw the current object
                glBindVertexArray(obj->m_mesh->VAO);
                glDrawArrays(GL_TRIANGLES, 0, (GLsizei)obj->m_mesh->fullVertexData.size() / 8);
            }
        }
    };
    
    auto SelectBuffer = [=, &lights_mframe](Framebuffer* buffer) {
        glViewport(0, 0, buffer->dimensions.x, buffer->dimensions.y);

        //Initialize the buffer
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->framebuffer);
        glEnable(GL_DEPTH_TEST); //Enable depthtest again
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        };

    auto DeSelectBuffer = []() {
        //De-initialize the current frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default render target
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        };

    /*SHADOW PASS*/

    const auto get_frustrum_corners = [_frustrum]() {
        const auto inv = glm::inverse(_frustrum);

        std::vector<Vector4> frustumCorners;
        for (unsigned int z = 0; z < 2; ++z)
            for (unsigned int y = 0; y < 2; ++y)
                for (unsigned int x = 0; x < 2; ++x)
                {
                    const glm::vec4 pt =
                        inv * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
        return frustumCorners;
        };

    const auto get_frustrum_center = [](std::vector<Vector4> corners) {
        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
        {
            center += glm::vec3(v);
        }
        center /= corners.size();

        return center;
    };

    std::vector<Vector4> frustrum_corners = get_frustrum_corners();
    auto frustrum_basis = std::vector<Vector3>();

    for (size_t i = 0; i < 4; i++)
    {
        frustrum_basis.push_back(-(Vector3)(frustrum_corners[i] - frustrum_corners[i + 4]));
    }

    auto get_subfrustrum_corners = [frustrum_basis, frustrum_corners](float from, float to) {
        auto subfrustrum_corners = std::vector<Vector4>();

        for (size_t i = 0; i < 4; i++)
        {
            Vector3 delta = frustrum_basis[i] * from;
            Vector3 final = (Vector3)frustrum_corners[i] + delta;
            subfrustrum_corners.push_back(Vector4(final.x, final.y, final.z, 1.0f));
        }

        for (size_t i = 0; i < 4; i++)
        {
            Vector3 delta = frustrum_basis[i] * to;
            Vector3 final = (Vector3)frustrum_corners[i] + delta;
            subfrustrum_corners.push_back(Vector4(final.x, final.y, final.z, 1.0f));
        }

        return subfrustrum_corners;
    };
    auto frustrum_center = get_frustrum_center(frustrum_corners);

    //DEBUG
    DirLight *mainlight;

    for (auto light : lights_this_frame)
    {
        if (light->GetType() == Light::DIRECTION) {
            auto dirlight = static_cast<DirLight*>(light);

            auto backtrack_dist = 20.0f;
            auto overshoot_dist = 100.0f;

            auto prev_split = 0.0f;
            Matrix4 higherResolution_avoid;

            for (size_t i = 0; i < dirlight->shadowmaps.size(); i++)
            {
                auto next_split = 1.0f;

                if (i < dirlight->cascade_splits.size())
                    next_split = dirlight->cascade_splits[i];

                auto subfrustrum_corners = get_subfrustrum_corners(prev_split, next_split);
                
                if (i < dirlight->cascade_splits.size())
                    prev_split = dirlight->cascade_splits[i];

                Matrix4 lightView = glm::lookAt(
                    frustrum_center - (dirlight->dir * backtrack_dist),
                    frustrum_center,
                    forward
                );

                float minX = std::numeric_limits<float>::max();
                float maxX = std::numeric_limits<float>::lowest();
                float minY = std::numeric_limits<float>::max();
                float maxY = std::numeric_limits<float>::lowest();
                float maxZ = std::numeric_limits<float>::lowest();
                for (const auto& v : subfrustrum_corners)
                {
                    const auto trf = lightView * v;
                    minX = std::min(minX, trf.x);
                    maxX = std::max(maxX, trf.x);
                    minY = std::min(minY, trf.y);
                    maxY = std::max(maxY, trf.y);
                    maxZ = std::max(maxZ, trf.z);
                }

                const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, 0.0f, maxZ + overshoot_dist);

                dirlight->cascade_projections[i] = lightProjection * lightView;

                SelectBuffer(dirlight->shadowmaps[i]);
                if (i > 0) {
                    depthShader->SetOverride("avoid_enabled", true);
                    depthShader->SetOverride("avoid_matrix", higherResolution_avoid);
                }

                render_scene_to_active_buffer(lightProjection * lightView, depthShader);
                
                depthShader->SetOverride("avoid_enabled", false);
                
                DeSelectBuffer();

                higherResolution_avoid = lightProjection * lightView;

                //DEBUG
                mainlight = dirlight;
            }
        }
    }

    /*MAIN PASS*/
    
    //Draw to the main display buffer
    SelectBuffer(mFrameBuffer);
    render_scene_to_active_buffer(_frustrum);
    DeSelectBuffer();
    //Draw to the depth buffer using a depth shader
    SelectBuffer(mDepthFrameBuffer);
    render_scene_to_active_buffer(_frustrum, depthShader);

    DeSelectBuffer();

    //Assign camera shader as post-processing
    auto camShaderId = postprocess->shaderID;
    glUseProgram(camShaderId);
    glBindVertexArray(mFrameBuffer->quadVAO);
    glDisable(GL_DEPTH_TEST); //Temporarily disable depth test

    //Attach the color texture to the post-process shader
    glActiveTexture(GL_TEXTURE0);
    postprocess->SetTextureOverride("colorBufferTexture", mFrameBuffer, 0);
    //Attach the depth texture to the post-process shader
    glActiveTexture(GL_TEXTURE1);
    postprocess->SetTextureOverride("depthBufferTexture", mDepthFrameBuffer, 1);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //RENDER GUI HERE
    Vector2 bl_pivot = Vector2(-1, -1);
    Vector2 tr_pivot = Vector2(-1, -1);

    Vector2 bl_offset = Vector2(50, 50);
    Vector2 tr_offset = Vector2(200, 100);

    Vector2 normalizer = this->resolution * 0.5f;
    Vector3 final_bl = Vector3(bl_pivot.x, bl_pivot.y, 0) + Vector3(bl_offset.x / normalizer.x, bl_offset.y / normalizer.y, 0);
    Vector3 final_tr = Vector3(tr_pivot.x, tr_pivot.y, 0) + Vector3(tr_offset.x / normalizer.x, tr_offset.y / normalizer.y, 0);
    Vector3 rect_center = (final_tr + final_bl) * 0.5f;
    Vector3 rect_scale = (final_tr - final_bl) * 0.5f;

    Matrix4 canvas_transform = Matrix4(1.0f);
    canvas_transform *= glm::translate(rect_center);
    canvas_transform *= glm::scale(rect_scale);
    

    glUseProgram(this->uiShader->shaderID);
    this->uiShader->SetOverride("color", Vector4(1, 1, 1, 1));
    this->uiShader->SetOverride("viewport_size", this->resolution);
    this->uiShader->SetOverride("bl_pivot", bl_pivot);
    this->uiShader->SetOverride("tr_pivot", tr_pivot);
    this->uiShader->SetOverride("bl_offset", bl_offset);
    this->uiShader->SetOverride("tr_offset", tr_offset);
    this->uiShader->SetOverride("transform", canvas_transform);


    glBindVertexArray(this->default_quad->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    this->lights_this_frame.clear();
#pragma endregion
}

void gbe::RenderPipeline::RegisterDrawCall(DrawCall* drawcall)
{
    this->drawcalls.push_back(drawcall);
}

void gbe::RenderPipeline::CleanUp()
{
    for (size_t o_i = 0; o_i < drawcalls.size(); o_i++)
    {
        glDeleteVertexArrays(1, &drawcalls[o_i]->m_mesh->VAO);
        glDeleteBuffers(1, &drawcalls[o_i]->m_mesh->VBO);
    }
}
