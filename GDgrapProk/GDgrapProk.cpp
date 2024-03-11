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

static class Player {
public:
    int lastKnownDepth = 9999;
    GDObject* Obj;
    glm::vec3 Velocity;
} mPlayer;

static std::vector<GDObject*> objs;

static GDWindow* mWindow;
static GDCamera* mCamera3rd;
static GDCamera* mCamera1st;
static GDCamera* mCameraOrtho;
static GDCamera* activeCamera;
static GDSkybox* mSkybox;

static glm::vec3 cam3rdOffset = glm::vec3(0, 2, 0);
static glm::vec3 cam1stOffset = glm::vec3(0, 0, 3);
static glm::vec3 camOrthoOffset = glm::vec3(0, 40, 0);
static glm::vec3 camOrthoCenter = glm::vec3(0, 0, 0);
static float camDistance = 10;
static glm::vec2 cam3rdOrbitalPos = glm::vec2(0, 0);
static glm::vec2 camOrthoOrbitalPos = glm::vec2(-0.01f, 0);

bool queue_cam1switch = false;
bool queue_cam2switch = false;
bool queue_lightswitch = false;

void CameraControl(float speed)
{
    if (glfwGetKey(mWindow->window, GLFW_KEY_1) == GLFW_PRESS)
        queue_cam1switch = true;
    if (glfwGetKey(mWindow->window, GLFW_KEY_1) == GLFW_RELEASE && queue_cam1switch) {
        queue_cam1switch = false;

        if (activeCamera != mCamera1st)
            activeCamera = mCamera1st;
        else
            activeCamera = mCamera3rd;
    }

    if (glfwGetKey(mWindow->window, GLFW_KEY_2) == GLFW_PRESS)
        queue_cam2switch = true;
    if (glfwGetKey(mWindow->window, GLFW_KEY_2) == GLFW_RELEASE && queue_cam2switch) {
        queue_cam2switch = false;

        if (activeCamera != mCameraOrtho) {
            activeCamera = mCameraOrtho;
            camOrthoCenter = mPlayer.Obj->pos;
            camOrthoCenter.y = 0;
            camOrthoOrbitalPos = glm::vec2(-0.01f, 0);
        }
        else
            activeCamera = mCamera3rd;
    }

    if (activeCamera == mCamera3rd) {
        auto camLookAtPos = mPlayer.Obj->pos + cam3rdOffset;

        cam3rdOrbitalPos += mouseDelta;

        cam3rdOrbitalPos.y = glm::clamp(cam3rdOrbitalPos.y, -45.0f, 45.0f);

        auto dollyVector = EulerToVec(glm::vec3(0, 0, 1), cam3rdOrbitalPos) * camDistance;
        mCamera3rd->cameraPos = mPlayer.Obj->pos + dollyVector;
        mCamera3rd->CamF = glm::normalize(camLookAtPos - mCamera3rd->cameraPos);
    }

    auto playerForward = EulerToVec(glm::vec3(0, 0, 1), mPlayer.Obj->rot);
    mCamera1st->cameraPos = mPlayer.Obj->pos + EulerToVec(cam1stOffset, mPlayer.Obj->rot);
    mCamera1st->CamF = playerForward;

    if (activeCamera == mCameraOrtho) {
        float orthoDragMult = 0.4f * speed;
        
        if (glfwGetKey(mWindow->window, GLFW_KEY_UP) == GLFW_PRESS)
            camOrthoCenter += glm::vec3(0, 0, -1) * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camOrthoCenter += glm::vec3(0, 0, 1) * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camOrthoCenter += glm::vec3(-1, 0, 0) * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camOrthoCenter += glm::vec3(1, 0, 0) * orthoDragMult;

        auto camLookAtPos = camOrthoCenter;
        //camOrthoOrbitalPos.x += mouseDelta.x;
        
        if (glfwGetMouseButton(mWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            camOrthoOrbitalPos.y += mouseDelta.y * 0.5f;
            camOrthoOrbitalPos.x += -mouseDelta.x * 0.5f;
        }

        camOrthoOrbitalPos.y = glm::clamp(camOrthoOrbitalPos.y, -160.0f, -0.01f);

        auto dollyVector = RotVec(-camOrthoOffset, glm::vec3(-1, 0, 0), camOrthoOrbitalPos.y);
        dollyVector = RotVec(dollyVector, glm::vec3(0, 1, 0), camOrthoOrbitalPos.x);
        mCameraOrtho->cameraPos = camOrthoCenter - dollyVector;
        mCameraOrtho->CamF = glm::normalize(camLookAtPos - mCameraOrtho->cameraPos);
    }
}

void MovementControl(float acceleration) {

    auto playerForward = EulerToVec(glm::vec3(0, 0, 1), mPlayer.Obj->rot);

    if (glfwGetKey(mWindow->window, GLFW_KEY_W) == GLFW_PRESS)
        mPlayer.Velocity += playerForward * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_S) == GLFW_PRESS)
        mPlayer.Velocity -= playerForward * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_Q) == GLFW_PRESS)
        mPlayer.Velocity -= glm::vec3(0, 1, 0) * acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_E) == GLFW_PRESS)
        mPlayer.Velocity += glm::vec3(0, 1, 0) * acceleration;
}

void TurnControl(float acceleration) {
    if (glfwGetKey(mWindow->window, GLFW_KEY_A) == GLFW_PRESS)
        mPlayer.Obj->rot.x += acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_D) == GLFW_PRESS)
        mPlayer.Obj->rot.x -= acceleration;
}



int main(void)
{
    std::cout << "ToDo : Print depth and limit depth to max=0" << std::endl;
    std::cout << "ToDo : Normal map player ship" << std::endl;
    std::cout << "ToDo : Post process mono shading" << std::endl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    mWindow = new GDWindow();

    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

#pragma region Others setup
    //Skybox setup
    mSkybox = new GDSkybox();
    mSkybox->shader = new GDShader("Shaders/skybox.vert", "Shaders/skybox.frag");
    mSkybox->textureCubeMap = new GDTextureCubeMap("Tex/Skybox/uw", "jpg");
    
    //Shaders
    auto litShader = new GDShader("Shaders/lit.vert", "Shaders/lit.frag");
    auto Cam3rdPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto Cam1stPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto CamOrthoPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    
    //Framebuffer
    auto mFrameBuffer = new GDFramebuffer(mWindow);

    //Camera Setup
    mCamera3rd = new GDCamera(mWindow, Cam3rdPPShader);
    glUseProgram(Cam3rdPPShader->shaderID);
    glUniform1f(glGetUniformLocation(Cam3rdPPShader->shaderID, "saturation"), 0.8f);
    glUniform1f(glGetUniformLocation(Cam3rdPPShader->shaderID, "blur"), 0.01f);
    glUniform4fv(glGetUniformLocation(Cam3rdPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1) * 0.7f));
    mCamera3rd->setPerspectiveFOV(80.0f, 20);

    mCamera1st = new GDCamera(mWindow, Cam1stPPShader);
    glUseProgram(Cam1stPPShader->shaderID);
    glUniform1f(glGetUniformLocation(Cam1stPPShader->shaderID, "saturation"), 0.0f);
    glUniform4fv(glGetUniformLocation(Cam1stPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));
    mCamera1st->setPerspectiveFOV(80.0f, 100);

    mCameraOrtho = new GDCamera(mWindow, CamOrthoPPShader);
    glUseProgram(CamOrthoPPShader->shaderID);
    glUniform1f(glGetUniformLocation(CamOrthoPPShader->shaderID, "saturation"), 0.0f);
    glUniform4fv(glGetUniformLocation(CamOrthoPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));
    auto orthoRange = 30.0f;
    mCameraOrtho->proj = glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, 0.01f, 100.0f);
    mCameraOrtho->cameraPos = glm::vec3(0, 40, 0);
    mCameraOrtho->CamF = glm::vec3(0, -1, 0);
    mCameraOrtho->WorldUp = glm::vec3(0, 1, 0);

    activeCamera = mCamera3rd;

    //Light setup
    GDPointLight playerPointLight;
    playerPointLight.color = glm::vec3(0.5, 1, 0.5);
    playerPointLight.intensity = 30;
    GDDirLight dirLight;
    dirLight.dir = glm::vec3(0.3, -1, 0);
    dirLight.color= glm::vec3(0.7, 0.6, 1) * 0.6f;
#pragma endregion

#pragma region Object setup
    //Submarines setup
    mPlayer.Obj = new GDObject("3D/sub1.obj");
    mPlayer.Obj->shader = litShader;
    mPlayer.Obj->texturediff = new GDTexture("Tex/sub1/diff.png");
    mPlayer.Obj->pos = glm::vec3(0, -10, 0);
    mPlayer.Obj->scale *= 0.2f;
    objs.push_back(mPlayer.Obj);

    GDObject* newsub;

    newsub = new GDObject("3D/sub2.obj");
    newsub->shader = litShader;
    newsub->texturediff = new GDTexture("Tex/sub2/diff.jpg");
    newsub->scale *= 0.3f;
    newsub->pos = glm::vec3(-15, -17, -30);
    newsub->rot = glm::vec3(-50, 0, 0);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub3.obj");
    newsub->shader = litShader;
    newsub->texturediff = new GDTexture("Tex/sub3/diff.png");
    newsub->scale *= 3.0f;
    newsub->pos = glm::vec3(30, -6, -7);
    newsub->rot = glm::vec3(50, 0, 0);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub4.obj");
    newsub->shader = litShader;
    newsub->texturediff = new GDTexture("Tex/sub4/diff.jpg");
    newsub->scale *= 2.0f;
    newsub->pos = glm::vec3(-15, -30, 15);
    newsub->rot = glm::vec3(220, 0, 0);
    objs.push_back(newsub);

    auto sub2 = new GDObject("3D/sub5.obj");
    sub2->shader = litShader;
    sub2->texturediff = new GDTexture("Tex/sub5/diff.png");
    sub2->scale *= 2.0f;
    sub2->pos = glm::vec3(-15, -10, 15);
    objs.push_back(sub2);

    newsub = new GDObject("3D/sub6.obj");
    newsub->shader = litShader;
    newsub->texturediff = new GDTexture("Tex/sub6/diff.png");
    newsub->scale *= 0.05f;
    newsub->pos = glm::vec3(10, -20, 13);
    newsub->rot = glm::vec3(-30, 0, 0);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub7.obj");
    newsub->shader = litShader;
    newsub->texturediff = new GDTexture("Tex/sub7/diff.png");
    newsub->scale *= 3.0f;
    newsub->pos = glm::vec3(-20, -10, -12);
    newsub->rot = glm::vec3(40, 0, 0);
    objs.push_back(newsub);
#pragma endregion

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow->window))
    {
        //Update Updatables
        currentFrameT = glfwGetTime();
        deltaTime = currentFrameT - lastFrameT;
        lastFrameT = currentFrameT;

        glfwGetCursorPos(mWindow->window, &currentFrameMousePos.x, &currentFrameMousePos.y);
        mouseDelta = currentFrameMousePos - lastFrameMousePos;
        lastFrameMousePos = currentFrameMousePos;

#pragma region Player/Camera Control
        CameraControl(deltaTime * 20);
        
        if (activeCamera != mCameraOrtho) {
            TurnControl(deltaTime * 20);
            MovementControl(deltaTime * 0.004f);
        }

        mPlayer.Velocity -= (mPlayer.Velocity * 0.1f) * (deltaTime * 10);
        mPlayer.Obj->pos += mPlayer.Velocity;
        mPlayer.Obj->pos.y = fmin(mPlayer.Obj->pos.y, 0);

        if (mPlayer.lastKnownDepth != (int)mPlayer.Obj->pos.y) {
            mPlayer.lastKnownDepth = (int)mPlayer.Obj->pos.y;
            std::cout << "Current submarine depth: " << mPlayer.lastKnownDepth << std::endl;
        }

        //Player Light Control
        playerPointLight.pos = mPlayer.Obj->pos + EulerToVec(cam1stOffset, mPlayer.Obj->rot);
        if (glfwGetKey(mWindow->window, GLFW_KEY_F) == GLFW_PRESS)
            queue_lightswitch = true;
        if (glfwGetKey(mWindow->window, GLFW_KEY_F) == GLFW_RELEASE && queue_lightswitch) {
            queue_lightswitch = false;

            playerPointLight.intensity += 20;
            playerPointLight.intensity = (int)playerPointLight.intensity % 60;
        }

        glm::mat4 viewMat = glm::lookAt(activeCamera->cameraPos, activeCamera->CamF + activeCamera->cameraPos, activeCamera->WorldUp);
#pragma endregion

        /* =============== Render here =============== */

        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer->framebuffer);
        glEnable(GL_DEPTH_TEST); //Enable depthtest again
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (activeCamera != mCameraOrtho) {
            mSkybox->Render(viewMat, activeCamera->proj);
        }

#pragma endregion

        for (auto obj : objs)
        {
            glUseProgram(obj->shader->shaderID);

            glActiveTexture(GL_TEXTURE1);
            GLuint texdiffAddress = glGetUniformLocation(obj->shader->shaderID, "texdiffuse");
            glBindTexture(GL_TEXTURE_2D, obj->texturediff->texID);
            glUniform1i(texdiffAddress, 1);

            /*
            glActiveTexture(GL_TEXTURE2);
            GLuint texnormalAddress = glGetUniformLocation(obj->shader->shaderID, "texnormal");
            glBindTexture(GL_TEXTURE_2D, obj->texturenormal->texID);
            glUniform1i(texnormalAddress, 2);
            */

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
            glm::mat4 tmat_PVM = activeCamera->proj * viewMat * tmat;
            setMat("transform_model", tmat);
            setMat("transform_projection", tmat_PVM);

            setVec3("dLightDir", dirLight.dir);
            setVec3("dLightColor", dirLight.color);
            setVec3("pLightPos", playerPointLight.pos);
            setVec3("pLightColor", playerPointLight.color * playerPointLight.intensity);
            setFloat("ambientLightIntensity", 0.2f);

            setVec3("cameraPos", activeCamera->cameraPos);
            setFloat("specStrength", 0.9f);
            setFloat("specPhong", 12);

            glBindVertexArray(obj->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj->fullVertexData.size() / 8);
        }

        // second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(activeCamera->mShader->shaderID);
        glBindVertexArray(mFrameBuffer->quadVAO);
        glDisable(GL_DEPTH_TEST); //Temporarily disable depth test
        glBindTexture(GL_TEXTURE_2D, mFrameBuffer->textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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