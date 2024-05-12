#include "RenderPipeline.h"

gde::RenderPipeline::RenderPipeline(glm::vec2 dimensions, Camera* activeCamera)
{
    //Skybox setup
    this->mSkybox = NULL;

    //Framebuffers setup
    depthShader = new Shader("Shaders/object.vert", "Shaders/depth.frag");
    mFrameBuffer = new Framebuffer(dimensions);
    mDepthFrameBuffer = new Framebuffer(dimensions);

    this->activeCamera = activeCamera;
}

void gde::RenderPipeline::RenderFrame()
{
    //Update delta time
    currentFrameT = glfwGetTime();
    deltaTime = currentFrameT - lastFrameT;
    lastFrameT = currentFrameT;

#pragma region Rendering
    /* =============== Render here =============== */
    //Set up view matrix
    glm::mat4 viewMat = glm::lookAt(activeCamera->cameraPos, activeCamera->CamF + activeCamera->cameraPos, activeCamera->WorldUp);

    //Function to draw all objects to a specified buffer
    auto DrawToBuffer = [=](Framebuffer* buffer, bool depthWritersOnly = false, Shader* overrideShader = NULL) {
        //Initialize the buffer
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->framebuffer);
        glEnable(GL_DEPTH_TEST); //Enable depthtest again
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render the skybox if specified
        if (!depthWritersOnly && mSkybox != NULL) {
            mSkybox->Render(viewMat, activeCamera->getproj());
        }

        //Loop through all objects
        for (auto obj : drawcalls)
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
            //Transform data
            glm::mat4 tmat = glm::mat4(1.0f);
            tmat = glm::translate(tmat, obj->pos);
            tmat = glm::scale(tmat, obj->scale);
            tmat = glm::rotate(tmat, glm::radians(obj->rot.y), glm::vec3(0, 1, 0));
            tmat = glm::rotate(tmat, glm::radians(obj->rot.x), glm::vec3(1, 0, 0));
            tmat = glm::rotate(tmat, glm::radians(obj->rot.z), glm::vec3(0, 0, 1));
            glm::mat4 tmat_VM = tmat;
            glm::mat4 tmat_PVM = activeCamera->getproj() * viewMat * tmat;
            setMat4("transform_model", tmat);
            setMat4("transform_projection", tmat_PVM);
            setVec3("cameraPos", activeCamera->cameraPos);

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
            glDrawArrays(GL_TRIANGLES, 0, obj->m_mesh->fullVertexData.size() / 8);
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
    auto camShaderId = activeCamera->mShader->shaderID;
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
