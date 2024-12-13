#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "RenderPipeline.h"

using namespace gbe;

void gbe::RenderPipeline::SetRequiredAttribs()
{
}

gbe::RenderPipeline::RenderPipeline(void* (*procaddressfunc)(const char*), Vector2 dimensions)
{
    this->resolution = dimensions;
    auto thing = procaddressfunc("glGetString");

    if (gladLoadGLLoader(procaddressfunc) == 0)
        throw "ERROR: glad failed to initialize.";

    this->from = glm::vec3(1.0f);
    this->projMat = Matrix4();
    this->viewMat = Matrix4();
    this->postprocess = nullptr;

    //Skybox setup
    this->mSkybox = NULL;

    //Framebuffers setup
    mFrameBuffer = new Framebuffer(dimensions);
    mDepthFrameBuffer = new Framebuffer(dimensions);

    this->SetRequiredAttribs();
}

void gbe::RenderPipeline::Set_DepthShader(Shader* value)
{
    this->depthShader = value;
}

void RenderPipeline::SetMaximumLights(int maxlights) {
    this->maxlights = maxlights;
}

void RenderPipeline::SetView(Vector3 from, Matrix4 viewMat, Matrix4 projMat) {
    this->from = from;
    this->viewMat = viewMat;
    this->projMat = projMat;
}
void gbe::RenderPipeline::SetSkybox(Skybox* value)
{
    this->mSkybox = value;
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

Matrix4 gbe::RenderPipeline::GetProjMat() {
    return this->projMat;
}

void gbe::RenderPipeline::RenderFrame()
{
#pragma region Rendering
    auto& lights_mframe = this->lights_this_frame;
    /* =============== Render here =============== */
    const auto render_scene_to_active_buffer = [=,&lights_mframe](Matrix4 tmat_PV, Shader* overrideShader = nullptr) {
        int last_used_texture_slot = -1;
        const auto SelectEmptyTextureSlot = [&last_used_texture_slot]() {
            last_used_texture_slot++;
            glActiveTexture(GL_TEXTURE0 + last_used_texture_slot);
            return last_used_texture_slot;
        };
        
        //Loop through all objects
        for (auto& obj : drawcalls)
        {
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

            //Attach skybox texture for ambient lighting
            if (false) {
                auto tex_slot = SelectEmptyTextureSlot();
                curshader->SetTextureOverride("skybox", mSkybox->textureCubeMap, tex_slot);
            }

            //Pass the necessary CPU-computed data to the object shader

            for (auto& call : obj->calls) {

                size_t index = 0;
                for (auto lightdata : lights_mframe)
                {
                    if (lightdata->changed == false)
                        continue;

                    std::string index_fix = "[" + std::to_string(index) + "]";
                    curshader->SetOverride(("lights" + index_fix + ".enabled").c_str(), true);

                    if (lightdata->GetType() == Light::DIRECTION) {
                        auto dirlight = (DirLight*)lightdata;

                        curshader->SetOverride(("lights" + index_fix + ".type").c_str(), (int)Light::DIRECTION);
                        curshader->SetOverride(("lights" + index_fix + ".color").c_str(), (Vector3)(dirlight->color * dirlight->intensity));
                        curshader->SetOverride(("lights" + index_fix + ".dir").c_str(), dirlight->dir);
                        
                        curshader->SetOverride(("light_view_matrixes" + index_fix).c_str(), lightdata->view_projection_matrix);

                        auto tex_slot = SelectEmptyTextureSlot();
                        curshader->SetTextureOverride(("lightviews" + index_fix).c_str(), dirlight->shadowmap, tex_slot);
                    }

                    index++;
                }
                for (size_t cont = index; cont < 10; cont++)
                {
                    std::string index_fix = "[" + std::to_string(cont) + "]";
                    curshader->SetOverride(("lights" + index_fix + ".enabled").c_str(), false);
                }

                //Transform data
                Matrix4 tmat = call.second;
                curshader->SetOverride("transform_model", tmat);
                curshader->SetOverride("transform_projection", tmat_PV);
                curshader->SetOverride("cameraPos", from);

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

    const auto get_frustrum_corners = [](const glm::mat4& proj, const glm::mat4& view) {
        const auto inv = glm::inverse(proj * view);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt =
                        inv * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }
        return frustumCorners;
    };

    const auto get_frustrum_center = [](std::vector<glm::vec4> corners) {
        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
        {
            center += glm::vec3(v);
        }
        center /= corners.size();

        return center;
    };

    auto frustrum_corners = get_frustrum_corners(projMat, viewMat);
    auto frustrum_center = get_frustrum_center(frustrum_corners);

    Matrix4 main_light_pv;
    Framebuffer* main_light_depthbuffer = nullptr;

    for (auto light : lights_this_frame)
    {
        if (light->GetType() == Light::DIRECTION) {
            auto dirlight = static_cast<DirLight*>(light);

            auto backtrack_dist = 20.0f;
            auto overshoot_dist = 100.0f;

            const auto lightView = glm::lookAt(
                frustrum_center - (dirlight->dir * backtrack_dist),
                frustrum_center,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float maxZ = std::numeric_limits<float>::lowest();
            for (const auto& v : frustrum_corners)
            {
                const auto trf = lightView * v;
                minX = std::min(minX, trf.x);
                maxX = std::max(maxX, trf.x);
                minY = std::min(minY, trf.y);
                maxY = std::max(maxY, trf.y);
                maxZ = std::max(maxZ, trf.z);
            }

            const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, 0.0f, maxZ + overshoot_dist);

            main_light_pv = lightProjection * lightView;

            dirlight->view_projection_matrix = main_light_pv;

            SelectBuffer(dirlight->shadowmap);
            render_scene_to_active_buffer(main_light_pv, depthShader);
            DeSelectBuffer();

            main_light_depthbuffer = dirlight->shadowmap;
        }
    }

    /*MAIN PASS*/
    
    //Draw to the main display buffer
    SelectBuffer(mFrameBuffer);
    if (mSkybox != NULL) {
        //mSkybox->Render(viewMat, projMat);
    }
    render_scene_to_active_buffer(projMat * viewMat);
    DeSelectBuffer();
    //Draw to the depth buffer using a depth shader
    SelectBuffer(mDepthFrameBuffer);
    render_scene_to_active_buffer(projMat * viewMat, depthShader);

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

    //Output the buffer
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
