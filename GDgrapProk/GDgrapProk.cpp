#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "GD_Graphics/GDutil.h"
#include "GD_Graphics/GDShader.h"
#include "GD_Graphics/GDTexture.h"
#include "GD_Graphics/GDObject.h"
#include "GD_Graphics/GDWindow.h"
#include "GD_Graphics/GDCamera.h"
#include "GD_Graphics/GDSkybox.h"
#include "GD_Graphics/GDLight.h"
#include "GD_Graphics/GDFramebuffer.h"

auto lastFrameT = 0.0f;
auto currentFrameT = 0.0f;
auto deltaTime = 0.0f;

glm::dvec2 lastFrameMousePos;
glm::dvec2 currentFrameMousePos;
glm::dvec2 mouseDelta;

static std::vector<GDObject*> objs;

static GDWindow* mWindow;
static GDCamera* activeCamera;
static GDSkybox* mSkybox;


int main(void)
{
    /* Initialize GLFW*/
    if (!glfwInit())
        return -1;
    mWindow = new GDWindow(900, 900);

    /* Initialize GLAD*/
    gladLoadGL();

#pragma region Rendering Requisites Setup
    //Skybox setup
    mSkybox = NULL;
    
    //Shaders setup
    auto depthShader = new GDShader("Shaders/object.vert", "Shaders/depth.frag");
    auto litShader = new GDShader("Shaders/object.vert", "Shaders/lit.frag");
    auto unlitShader = new GDShader("Shaders/object.vert", "Shaders/unlit.frag");
    auto Cam3rdPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto Cam1stPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto CamOrthoPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    
    //Framebuffers setup
    auto mFrameBuffer = new GDFramebuffer(mWindow);
    auto mDepthFrameBuffer = new GDFramebuffer(mWindow);

    auto mCameraOrtho = new GDOrthographicCamera(mWindow, CamOrthoPPShader);
    glUseProgram(CamOrthoPPShader->shaderID);
    glUniform1f(glGetUniformLocation(CamOrthoPPShader->shaderID, "saturation"), 1.0f);
    glUniform4fv(glGetUniformLocation(CamOrthoPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1)));
    mCameraOrtho->orthoRange = 5.0f;
    mCameraOrtho->farClip = 200.0f;
    mCameraOrtho->cameraPos = glm::vec3(0, 0, -50);
    mCameraOrtho->CamF = glm::vec3(0, 0, 1);
    mCameraOrtho->WorldUp = glm::vec3(0, 1, 0);

    activeCamera = mCameraOrtho;
#pragma endregion

#pragma region Object setup
    //light obj setup
    auto mPLightObj = new GDObject("3D/sphere.obj",
        unlitShader,
        NULL,
        NULL,
        glm::vec3(0, 0, 0)
        );
    objs.push_back(mPLightObj);

#pragma endregion

    /// <summary>
    /// MAIN GAME LOOP
    /// </summary>
    while (!glfwWindowShouldClose(mWindow->window))
    {
        //Update delta time
        currentFrameT = glfwGetTime();
        deltaTime = currentFrameT - lastFrameT;
        lastFrameT = currentFrameT;

        //Update delta mouse
        glfwGetCursorPos(mWindow->window, &currentFrameMousePos.x, &currentFrameMousePos.y);
        mouseDelta = currentFrameMousePos - lastFrameMousePos;
        lastFrameMousePos = currentFrameMousePos;

#pragma region Rendering
        /* =============== Render here =============== */
        //Set up view matrix
        glm::mat4 viewMat = glm::lookAt(activeCamera->cameraPos, activeCamera->CamF + activeCamera->cameraPos, activeCamera->WorldUp);

        //Function to draw all objects to a specified buffer
        auto DrawToBuffer = [=](GDFramebuffer* buffer, bool depthWritersOnly = false, GDShader* overrideShader = NULL) {
            //Initialize the buffer
            glBindFramebuffer(GL_FRAMEBUFFER, buffer->framebuffer);
            glEnable(GL_DEPTH_TEST); //Enable depthtest again
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Render the skybox if specified
            if (activeCamera != mCameraOrtho && !depthWritersOnly && mSkybox != NULL) {
                mSkybox->Render(viewMat, activeCamera->getproj());
            }

            //Loop through all objects
            for (auto obj : objs)
            {
                //Enable chosen shader program
                if (overrideShader == NULL)
                    glUseProgram(obj->shader->shaderID);
                else
                    glUseProgram(overrideShader->shaderID);

#pragma region Shader Set Functions
                //Helper shader set functions
                auto setBool = [obj](const char* id, bool value) {
                    unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                    glUniform1i(xLoc, value);
                };
                auto setFloat = [obj](const char* id, float value) {
                    unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                    glUniform1f(xLoc, value);
                };
                auto setMat = [obj](const char* id, glm::mat4 value) {
                    unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                    glUniformMatrix4fv(xLoc, 1, GL_FALSE, glm::value_ptr(value));
                };
                auto setVec3 = [obj](const char* id, glm::vec3 value) {
                    unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                    glUniform3fv(xLoc, 1, glm::value_ptr(value));
                };
                auto setVec2 = [obj](const char* id, glm::vec2 value) {
                    unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                    glUniform2fv(xLoc, 1, glm::value_ptr(value));
                };
#pragma endregion

                //Attach color texture
                if (obj->texturediff != NULL) {
                    glActiveTexture(GL_TEXTURE1);
                    GLuint texdiffAddress = glGetUniformLocation(obj->shader->shaderID, "texdiffuse");
                    glBindTexture(GL_TEXTURE_2D, obj->texturediff->texID);
                    glUniform1i(texdiffAddress, 1);
                }

                //Attach normal texture if there is
                if (obj->texturenormal != NULL) {
                    glActiveTexture(GL_TEXTURE2);
                    GLuint texnormalAddress = glGetUniformLocation(obj->shader->shaderID, "texnormal");
                    glBindTexture(GL_TEXTURE_2D, obj->texturenormal->texID);
                    glUniform1i(texnormalAddress, 2);
                    setBool("hasNormalTex", true);
                }
                else {
                    setBool("hasNormalTex", false);
                }

                //Attach skybox texture for ambient lighting
                if (mSkybox != NULL) {
                    glActiveTexture(GL_TEXTURE0);
                    GLuint skyboxAddress = glGetUniformLocation(obj->shader->shaderID, "skybox");
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
                setMat("transform_model", tmat);
                setMat("transform_projection", tmat_PVM);
                setVec3("cameraPos", activeCamera->cameraPos);

                //Light data
                if (obj->shader == litShader) {
                    setFloat("specStrength", 2.9f);
                    setFloat("specPhong", 7);
                }
                else if (obj->shader == unlitShader) {
                    setVec3("color", glm::vec3(1, 0, 0));
                }

                //Draw the current object
                glBindVertexArray(obj->VAO);
                glDrawArrays(GL_TRIANGLES, 0, obj->fullVertexData.size() / 8);
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
        glfwSwapBuffers(mWindow->window);
        /* Poll for and process events */
        glfwPollEvents();
#pragma endregion
    }

    //Clean up
    for (size_t o_i = 0; o_i < objs.size(); o_i++)
    {
        glDeleteVertexArrays(1, &objs[o_i]->VAO);
        glDeleteBuffers(1, &objs[o_i]->VBO);
    }
    
    glfwTerminate();
    return 0;
}