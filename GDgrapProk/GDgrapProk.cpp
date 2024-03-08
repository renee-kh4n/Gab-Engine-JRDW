#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "GD_Graphics/GDutil.h"
#include "GD_Graphics/GDShader.h"
#include "GD_Graphics/GDTexture.h"
#include "GD_Graphics/GDObject.h"
#include "GD_Graphics/GDWindow.h"
#include "GD_Graphics/GDCamera.h"
#include "GD_Graphics/GDSkybox.h"



static GDObject* playerObj;
static glm::vec3 playerVelocity;

static std::vector<GDObject*> objs;

static GDWindow* mWindow;
static GDCamera* mCamera;
static GDSkybox* mSkybox;

static glm::vec3 camFollowOffset = glm::vec3(0, 2, 0);
static float camDistance = 10;
static glm::vec2 camOrbitalPos = glm::vec2(0, 0);

void CameraControl(float speed)
{
    auto camLookAtPos = playerObj->pos + camFollowOffset;
    auto maxYDiff = 1;

    if (glfwGetKey(mWindow->window, GLFW_KEY_UP) == GLFW_PRESS)
        camOrbitalPos.y += speed;
    if (glfwGetKey(mWindow->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camOrbitalPos.y -= speed;
    if (glfwGetKey(mWindow->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camOrbitalPos.x += speed;
    if (glfwGetKey(mWindow->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camOrbitalPos.x -= speed;

    camOrbitalPos.y = glm::clamp(camOrbitalPos.y, -45.0f, 45.0f);

    auto dollyVector = glm::vec3(0, 0, 1) * camDistance;
    dollyVector = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), dollyVector), glm::radians(camOrbitalPos.y), glm::vec3(-1, 0, 0)) * glm::vec4(dollyVector, 0));
    dollyVector = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), dollyVector), glm::radians(camOrbitalPos.x), glm::vec3(0, 1, 0)) * glm::vec4(dollyVector, 0));

    mCamera->cameraPos = playerObj->pos + dollyVector;
    mCamera->CamF = glm::normalize(camLookAtPos - mCamera->cameraPos);
}

void MovementControl(float acceleration) {

    auto playerForward = glm::vec3(0, 0, -1);
    playerForward = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), playerForward), glm::radians(playerObj->rot.y), glm::vec3(-1, 0, 0)) * glm::vec4(playerForward, 0));
    playerForward = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), playerForward), glm::radians(playerObj->rot.x), glm::vec3(0, 1, 0)) * glm::vec4(playerForward, 0));

    if (glfwGetKey(mWindow->window, GLFW_KEY_W) == GLFW_PRESS)
        playerVelocity += playerForward * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_S) == GLFW_PRESS)
        playerVelocity -= playerForward * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        playerVelocity -= glm::vec3(0, 1, 0) * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        playerVelocity += glm::vec3(0, 1, 0) * acceleration;
}

void TurnControl(float acceleration) {
    if (glfwGetKey(mWindow->window, GLFW_KEY_A) == GLFW_PRESS)
        playerObj->rot.x += acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_D) == GLFW_PRESS)
        playerObj->rot.x -= acceleration;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    mWindow = new GDWindow();
    mCamera = new GDCamera(mWindow);

    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

#pragma region Others setup
    //Shaders
    auto litShader = new GDShader("Shaders/lit.vert", "Shaders/lit.frag");

    //Light setup
    glm::vec3 pLightPos = glm::vec3(0, 1, 1);
    glm::vec3 pLightColor = glm::vec3(1, 1, 1) * 0.0f;
    glm::vec3 dLightDir = glm::vec3(1, -1, 0);
    glm::vec3 dLightColor = glm::vec3(0.7, 0.6, 1) * 1.0f;
    glm::vec3 ambientLightColor = glm::vec3(0.5, 0.4, 0.05);
    float ambientLightIntensity = 0.3f;
#pragma endregion

#pragma region Object setup
    //Skybox setup
    mSkybox = new GDSkybox();
    mSkybox->shader = new GDShader("Shaders/skybox.vert", "Shaders/skybox.frag");
    mSkybox->textureCubeMap = new GDTextureCubeMap("Tex/Skybox/rainbow");
    
    //Submarines setup
    playerObj = new GDObject("3D/sub1.obj");
    playerObj->shader = litShader;
    playerObj->texturediff = new GDTexture("Tex/sub1/diff.png");
    playerObj->texturenormal = new GDTexture("Tex/sub1/norm.png");
    playerObj->scale *= 0.4f;
    objs.push_back(playerObj);

    auto sub2 = new GDObject("3D/sub2.obj");
    sub2->shader = litShader;
    sub2->texturediff = new GDTexture("Tex/sub2/diff.jpg");
    sub2->texturenormal = new GDTexture("Tex/sub2/norm.png");
    sub2->scale *= 0.1f;
    sub2->pos = glm::vec3(0, 0, -10);
    objs.push_back(sub2);
#pragma endregion

#pragma region Time
    auto lastFrameT = 0.0f;
    auto currentFrameT = 0.0f;
    auto deltaTime = 0.0f;
#pragma endregion

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow->window))
    {
        //Update Time
        currentFrameT = glfwGetTime();
        deltaTime = currentFrameT - lastFrameT;
        lastFrameT = currentFrameT;

#pragma region Player/Camera Control
        MovementControl(deltaTime * 0.001f);
        TurnControl(deltaTime * 20);
        CameraControl(deltaTime * 40);

        playerVelocity -= (playerVelocity * 0.1f) * (deltaTime * 10);
        playerObj->pos += playerVelocity;

        mCamera->setPerspectiveFOV(60.0f);
        glm::mat4 viewMat = glm::lookAt(mCamera->cameraPos, mCamera->CamF + mCamera->cameraPos, mCamera->WorldUp);
#pragma endregion

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Skybox
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(mSkybox->shader->shaderID);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMat));

        glUniformMatrix4fv(glGetUniformLocation(mSkybox->shader->shaderID, "view"), 1, GL_FALSE, glm::value_ptr(sky_view));
        glUniformMatrix4fv(glGetUniformLocation(mSkybox->shader->shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(mCamera->proj_pers));

        glBindVertexArray(mSkybox->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mSkybox->textureCubeMap->texID);
        glUniform1i(glGetUniformLocation(mSkybox->shader->shaderID, "skybox"), 0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
#pragma endregion

        for (auto obj : objs)
        {
            glUseProgram(obj->shader->shaderID);

            glActiveTexture(GL_TEXTURE1);
            GLuint texdiffAddress = glGetUniformLocation(obj->shader->shaderID, "texdiffuse");
            glBindTexture(GL_TEXTURE_2D, obj->texturediff->texID);
            glUniform1i(texdiffAddress, 1);

            glActiveTexture(GL_TEXTURE2);
            GLuint texnormalAddress = glGetUniformLocation(obj->shader->shaderID, "texnormal");
            glBindTexture(GL_TEXTURE_2D, obj->texturenormal->texID);
            glUniform1i(texnormalAddress, 2);

            glActiveTexture(GL_TEXTURE0);
            GLuint skyboxAddress = glGetUniformLocation(obj->shader->shaderID, "skybox");
            glBindTexture(GL_TEXTURE_2D, mSkybox->textureCubeMap->texID);
            glUniform1i(skyboxAddress, 0);

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

            glm::mat4 tmat = glm::mat4(1.0f);
            tmat = glm::translate(tmat, obj->pos);
            tmat = glm::scale(tmat, obj->scale);
            tmat = glm::rotate(tmat, glm::radians(obj->rot.x), glm::vec3(0, 1, 0));
            tmat = glm::rotate(tmat, glm::radians(obj->rot.y), glm::vec3(1, 0, 0));
            glm::mat4 tmat_VM = tmat;
            glm::mat4 tmat_PVM = mCamera->proj_pers * viewMat * tmat;
            setMat("transform_model", tmat);
            setMat("transform_projection", tmat_PVM);

            setVec3("dLightDir", dLightDir);
            setVec3("dLightColor", dLightColor);
            setVec3("pLightPos", pLightPos);
            setVec3("pLightColor", pLightColor);
            setVec3("ambientLightColor", ambientLightColor);
            setFloat("ambientLightIntensity", ambientLightIntensity);

            setVec3("cameraPos", mCamera->cameraPos);
            setFloat("specStrength", 0.9f);
            setFloat("specPhong", 12);

            glBindVertexArray(obj->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj->fullVertexData.size() / 8);
        }

        glfwSwapBuffers(mWindow->window);
        /* Poll for and process events */
        glfwPollEvents();
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