#include "RenderPipeline.h"

#include <glm/gtx/matrix_decompose.hpp>

using namespace gde;

gde::RenderPipeline::RenderPipeline(glm::vec2 dimensions)
{
    this->from = glm::vec3(1.0f);
    this->projMat = glm::mat4(1.0f);
    this->viewMat = glm::mat4(1.0f);
    this->postprocess = nullptr;

    //Skybox setup
    this->mSkybox = NULL;

    //Framebuffers setup
    depthShader = new Shader("Shaders/object.vert", "Shaders/depth.frag");
    mFrameBuffer = new Framebuffer(dimensions);
    mDepthFrameBuffer = new Framebuffer(dimensions);
}

void RenderPipeline::SetMaximumLights(int maxlights) {
    this->maxlights = maxlights;
}

void RenderPipeline::SetView(glm::vec3 from, glm::mat4 viewMat, glm::mat4 projMat) {
    this->from = from;
    this->viewMat = viewMat;
    this->projMat = projMat;
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

glm::mat4 gde::RenderPipeline::GetProjMat() {
    return this->projMat;
}

void gde::RenderPipeline::RenderFrame()
{
    //Update delta time
    currentFrameT = glfwGetTime();
    deltaTime = currentFrameT - lastFrameT;
    lastFrameT = currentFrameT;

#pragma region Rendering
    auto& lights_mframe = this->lights_this_frame;
    /* =============== Render here =============== */
    //Function to draw all objects to a specified buffer
    auto DrawToBuffer = [=, &lights_mframe](Framebuffer* buffer, bool depthWritersOnly = false, Shader* overrideShader = NULL) {
        //Initialize the buffer
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->framebuffer);
        glEnable(GL_DEPTH_TEST); //Enable depthtest again
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render the skybox if specified
        if (!depthWritersOnly && mSkybox != NULL) {
            mSkybox->Render(viewMat, projMat);
        }

        //Loop through all objects
        for (auto& obj : drawcalls)
        {
            //Enable chosen shader program
            if (overrideShader == NULL)
                glUseProgram(obj->m_material->m_shader->shaderID);
            else
                glUseProgram(overrideShader->shaderID);

#pragma region Shader Set Functions
            //Helper shader set functions
            auto setBool = [obj](const char* id, bool value) {
                unsigned int xLoc = glGetUniformLocation(obj->m_material->m_shader->shaderID, id);
                glUniform1i(xLoc, value);
            };
            auto setFloat = [obj](const char* id, float value) {
                unsigned int xLoc = glGetUniformLocation(obj->m_material->m_shader->shaderID, id);
                glUniform1f(xLoc, value);
            };
            auto setVec2 = [obj](const char* id, glm::vec2 value) {
                unsigned int xLoc = glGetUniformLocation(obj->m_material->m_shader->shaderID, id);
                glUniform2fv(xLoc, 1, glm::value_ptr(value));
            };
            auto setVec3 = [obj](const char* id, glm::vec3 value) {
                unsigned int xLoc = glGetUniformLocation(obj->m_material->m_shader->shaderID, id);
                glUniform3fv(xLoc, 1, glm::value_ptr(value));
            };
            auto setMat4 = [obj](const char* id, glm::mat4 value) {
                unsigned int xLoc = glGetUniformLocation(obj->m_material->m_shader->shaderID, id);
                glUniformMatrix4fv(xLoc, 1, GL_FALSE, glm::value_ptr(value));
            };
#pragma endregion
            for (int i = 0; i < obj->m_material->textureOverrides.size(); i++)
            {
                auto& textureOverride = obj->m_material->textureOverrides[i];

                glActiveTexture(GL_TEXTURE0 + i);
                GLuint texAddress = glGetUniformLocation(obj->m_material->m_shader->shaderID, textureOverride.parameterName);

                glBindTexture(GL_TEXTURE_2D, textureOverride.texture->texID);
                glUniform1i(texAddress, i);
            }

            //Attach skybox texture for ambient lighting
            if (mSkybox != NULL) {
                glActiveTexture(GL_TEXTURE0);
                GLuint skyboxAddress = glGetUniformLocation(obj->m_material->m_shader->shaderID, "skybox");
                glBindTexture(GL_TEXTURE_2D, mSkybox->textureCubeMap->texID);
                glUniform1i(skyboxAddress, 0);
            }

            //Pass the necessary CPU-computed data to the object shader
            
            for (auto& call : obj->calls) {

                size_t index = 0;
                for (auto lightdata : lights_mframe)
                {
                    if (lightdata->changed == false && lightdata->previous_render_index == index)
                        continue;

                    if (lightdata->GetType() == Light::DIRECTION) {
                        auto dirlight = (DirLight*)lightdata;
                        setVec3("dirlight.color", dirlight->color * dirlight->intensity);
                        setVec3("dirlight.dir", dirlight->dir);
                    }

                    index++;
                }
                //Transform data
                glm::mat4 tmat = call.second;
                glm::mat4 tmat_VM = tmat;
                glm::mat4 tmat_PVM = projMat * viewMat * tmat;
                setMat4("transform_model", tmat);
                setMat4("transform_projection", tmat_PVM);
                setVec3("cameraPos", from);

                for (auto& iterator : obj->m_material->overrides) {
                    switch (iterator.second.type)
                    {
                    case BOOL:
                        setBool(iterator.first, iterator.second.value_bool);
                        break;
                    case FLOAT:
                        setFloat(iterator.first, iterator.second.value_float);
                        break;
                    case VEC2:
                        setVec2(iterator.first, iterator.second.value_vec2);
                        break;
                    case VEC3:
                        setVec3(iterator.first, iterator.second.value_vec3);
                        break;
                    case MAT4:
                        setMat4(iterator.first, iterator.second.value_mat4);
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

        //De-initialize the current frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default render target
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    //Draw to the main display buffer
    DrawToBuffer(mFrameBuffer);
    //Draw to the depth buffer using a depth shader
    DrawToBuffer(mDepthFrameBuffer, true, depthShader);

    //Assign camera shader as post-processing
    auto camShaderId = postprocess->shaderID;
    glUseProgram(camShaderId);
    glBindVertexArray(mFrameBuffer->quadVAO);
    glDisable(GL_DEPTH_TEST); //Temporarily disable depth test

    //Attach the color texture to the post-process shader
    glActiveTexture(GL_TEXTURE0);
    GLuint colorBufferTextureSampler = glGetUniformLocation(camShaderId, "colorBufferTexture");
    glBindTexture(GL_TEXTURE_2D, mFrameBuffer->textureColorbuffer);
    glUniform1i(colorBufferTextureSampler, 0);
    //Attach the depth texture to the post-process shader
    glActiveTexture(GL_TEXTURE1);
    GLuint depthBufferTextureSampler = glGetUniformLocation(camShaderId, "depthBufferTexture");
    glBindTexture(GL_TEXTURE_2D, mDepthFrameBuffer->textureColorbuffer);
    glUniform1i(depthBufferTextureSampler, 1);

    //Output the buffer
    glDrawArrays(GL_TRIANGLES, 0, 6);

    this->lights_this_frame.clear();
#pragma endregion
}

void gde::RenderPipeline::RegisterDrawCall(DrawCall* drawcall)
{
    this->drawcalls.push_back(drawcall);
}

void gde::RenderPipeline::CleanUp()
{
    for (size_t o_i = 0; o_i < drawcalls.size(); o_i++)
    {
        glDeleteVertexArrays(1, &drawcalls[o_i]->m_mesh->VAO);
        glDeleteBuffers(1, &drawcalls[o_i]->m_mesh->VBO);
    }
}
