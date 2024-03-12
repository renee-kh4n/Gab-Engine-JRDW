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

class Torpedo {
public:
    GDObject* Obj;
    glm::vec3 Velocity = glm::vec3(0);
    glm::vec3 currentHeading;
    glm::vec3 target;
    float age;

    Torpedo(GDObject* Obj, glm::vec3 target, glm::vec3 currentHeading) {
        this->Obj = Obj;
        this->target = target;
        this->currentHeading = currentHeading;
    }
};

static std::vector<Torpedo*> torpedoes;
static std::vector<GDObject*> ships;
static std::vector<GDObject*> objs;

static GDWindow* mWindow;
static GDCamera* mCamera3rd;
static GDCamera* mCamera1st;
static GDCamera* mCameraOrtho;
static GDCamera* activeCamera;
static GDSkybox* mSkybox;

static glm::vec3 cam3rdOffset = glm::vec3(0, 2, 0);
static glm::vec3 cam1stOffset = glm::vec3(0, 0, 3);
static glm::vec3 camOrthoOffset = glm::vec3(0, 100, 0);
static glm::vec3 camOrthoCenter = glm::vec3(0, 0, 0);
static float camDistance = 10;
static glm::vec2 cam3rdOrbitalPos = glm::vec2(0, 0);
static glm::vec2 camOrthoOrbitalPos = glm::vec2(-0.01f, 0);

float torpedo_cd = 0;

bool queue_cam1switch = false;
bool queue_cam2switch = false;
bool queue_lightswitch = false;
bool queue_torpedo = false;

/// <summary>
/// Logic to control the three cameras using GLFW's input system.
/// </summary>
/// <param name="speed">Control speed</param>
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

        if (glfwGetMouseButton(mWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            cam3rdOrbitalPos.x += mouseDelta.y;
            cam3rdOrbitalPos.y += mouseDelta.x;
        }

        cam3rdOrbitalPos.x = glm::clamp(cam3rdOrbitalPos.x, -45.0f, 45.0f);

        auto dollyVector = EulerToVec(glm::vec3(0, 0, 1), cam3rdOrbitalPos) * camDistance;
        mCamera3rd->cameraPos = mPlayer.Obj->pos + dollyVector;
        mCamera3rd->CamF = glm::normalize(camLookAtPos - mCamera3rd->cameraPos);
    }

    auto playerForward = EulerToVec(glm::vec3(0, 0, 1), mPlayer.Obj->rot);
    mCamera1st->cameraPos = mPlayer.Obj->pos + EulerToVec(cam1stOffset, mPlayer.Obj->rot);
    mCamera1st->CamF = playerForward;

    if (activeCamera == mCameraOrtho) {
        auto camLookAtPos = camOrthoCenter;

        if (glfwGetMouseButton(mWindow->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            camOrthoOrbitalPos.x += mouseDelta.y * 0.5f;
            camOrthoOrbitalPos.y += -mouseDelta.x * 0.5f;
        }

        camOrthoOrbitalPos.x = glm::clamp(camOrthoOrbitalPos.x, -160.0f, -0.01f);

        auto dollyVector = RotVec(-camOrthoOffset, glm::vec3(-1, 0, 0), camOrthoOrbitalPos.x);
        dollyVector = RotVec(dollyVector, glm::vec3(0, 1, 0), camOrthoOrbitalPos.y);
        mCameraOrtho->cameraPos = camOrthoCenter - dollyVector;
        mCameraOrtho->CamF = glm::normalize(camLookAtPos - mCameraOrtho->cameraPos);

        float orthoDragMult = 0.4f * speed;
        
        if (glfwGetKey(mWindow->window, GLFW_KEY_UP) == GLFW_PRESS)
            camOrthoCenter += mCameraOrtho->CamUp() * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camOrthoCenter += -mCameraOrtho->CamUp() * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camOrthoCenter += -mCameraOrtho->CamR() * orthoDragMult;
        if (glfwGetKey(mWindow->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camOrthoCenter += mCameraOrtho->CamR() * orthoDragMult;
    }
}

/// <summary>
/// Change the player objects velocity through GLFW's input system.
/// </summary>
/// <param name="acceleration"></param>
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

/// <summary>
/// Change the player objects y-axis rotation through GLFW's input system.
/// </summary>
/// <param name="acceleration"></param>
void TurnControl(float acceleration) {
    if (glfwGetKey(mWindow->window, GLFW_KEY_A) == GLFW_PRESS)
        mPlayer.Obj->rot.y += acceleration;
    if (glfwGetKey(mWindow->window, GLFW_KEY_D) == GLFW_PRESS)
        mPlayer.Obj->rot.y -= acceleration;
}

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
    mSkybox = new GDSkybox();
    mSkybox->shader = new GDShader("Shaders/skybox.vert", "Shaders/skybox.frag");
    mSkybox->textureCubeMap = new GDTextureCubeMap("Tex/Skybox/uw", "jpg");
    
    //Shaders setup
    auto depthShader = new GDShader("Shaders/object.vert", "Shaders/depth.frag");
    auto litShader = new GDShader("Shaders/object.vert", "Shaders/lit.frag");
    auto Cam3rdPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto Cam1stPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto CamOrthoPPShader = new GDShader("Shaders/camshader.vert", "Shaders/camshader.frag");
    
    //Framebuffers setup
    auto mFrameBuffer = new GDFramebuffer(mWindow);
    auto mDepthFrameBuffer = new GDFramebuffer(mWindow);

    //Cameras and their postprocessing effects setup
    mCamera3rd = new GDCamera(mWindow, Cam3rdPPShader);
    glUseProgram(Cam3rdPPShader->shaderID);
    glUniform1f(glGetUniformLocation(Cam3rdPPShader->shaderID, "saturation"), 0.6f);
    glUniform1f(glGetUniformLocation(Cam3rdPPShader->shaderID, "blur"), 0.4f);
    glUniform2fv(glGetUniformLocation(Cam3rdPPShader->shaderID, "blurFromTo"), 1, glm::value_ptr(glm::vec2(13, 20)));
    glUniform4fv(glGetUniformLocation(Cam3rdPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1) * 0.7f));
    mCamera3rd->setPerspectiveFOV(80.0f, 25);

    mCamera1st = new GDCamera(mWindow, Cam1stPPShader);
    glUseProgram(Cam1stPPShader->shaderID);
    glUniform1f(glGetUniformLocation(Cam1stPPShader->shaderID, "saturation"), 0.0f);
    glUniform4fv(glGetUniformLocation(Cam1stPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));
    mCamera1st->setPerspectiveFOV(80.0f, 100);

    mCameraOrtho = new GDCamera(mWindow, CamOrthoPPShader);
    glUseProgram(CamOrthoPPShader->shaderID);
    glUniform1f(glGetUniformLocation(CamOrthoPPShader->shaderID, "saturation"), 0.0f);
    glUniform4fv(glGetUniformLocation(CamOrthoPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(0, 1, 0, 1)));
    auto orthoRange = 50.0f;
    mCameraOrtho->proj = glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, 1.0f, 200.0f);
    mCameraOrtho->cameraPos = glm::vec3(0, 40, 0);
    mCameraOrtho->CamF = glm::vec3(0, -1, 0);
    mCameraOrtho->WorldUp = glm::vec3(0, 1, 0);

    activeCamera = mCamera3rd;

    //Lights setup
    GDConeLight playerConeLight;
    playerConeLight.color = glm::vec3(0.5, 1, 0.5);
    playerConeLight.angle = glm::vec2(0, glm::radians(30.0f));
    playerConeLight.intensity = 30;
    GDDirLight dirLight;
    dirLight.dir = glm::vec3(0.3, -1, 0);
    dirLight.color= glm::vec3(0.7, 0.6, 1) * 0.8f;
#pragma endregion

#pragma region Object setup
    //PRELOAD: Torpedo Object
    auto torpedoObj = new GDObject("3D/sub3.obj",
        litShader,
        new GDTexture("Tex/sub1/diff.png"),
        new GDTexture("Tex/sub1/norm.png"),
        glm::vec3(0),
        glm::vec3(0.1f)
    );

    //Spawn torpedo function
    auto spawnTorpedo = [litShader, torpedoObj]() {

        glm::vec3 nearestPos = ships[0]->pos;

        for (auto ship : ships)
        {
            if (getSqrMag(ship->pos - mPlayer.Obj->pos) < getSqrMag(nearestPos - mPlayer.Obj->pos)) {
                nearestPos = ship->pos;
            }
        }

        auto newTorpedoObj = new GDObject(*torpedoObj);
        newTorpedoObj->pos = mPlayer.Obj->pos;

        auto newTorpedo = new Torpedo(newTorpedoObj, nearestPos, EulerToVec(glm::vec3(0, 0, 1), mPlayer.Obj->rot));
        torpedoes.push_back(newTorpedo);
        objs.push_back(newTorpedoObj);
    };

    //Player sub setup
    mPlayer.Obj = new GDObject("3D/sub1.obj",
        litShader,
        new GDTexture("Tex/sub1/diff.png"),
        new GDTexture("Tex/sub1/norm.png"),
        glm::vec3(0, -10, 0), glm::vec3(0.01f)
    );
    objs.push_back(mPlayer.Obj);

    //Other enemy submarines setup
    GDObject* newsub;

    newsub = new GDObject("3D/sub2.obj",
        litShader,
        new GDTexture("Tex/sub2/diff.jpg"),
        new GDTexture("Tex/sub2/norm.png"),
        glm::vec3(-15, -17, -30), glm::vec3(0.3f), glm::vec3(0, -50, 0)
        );
    ships.push_back(newsub);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub3.obj",
        litShader,
        new GDTexture("Tex/sub3/diff.png"),
        new GDTexture("Tex/sub3/norm.png"),
        glm::vec3(30, -6, -7), glm::vec3(3.0f), glm::vec3(0, 50, 0)
    );
    ships.push_back(newsub);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub4.obj",
        litShader,
        new GDTexture("Tex/sub4/diff.jpg"),
        new GDTexture("Tex/sub4/norm.jpg"),
        glm::vec3(-15, -30, 15), glm::vec3(2.0f), glm::vec3(0, 220, 0)
    );
    ships.push_back(newsub);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub5.obj",
        litShader,
        new GDTexture("Tex/sub5/diff.png"),
        NULL,
        glm::vec3(-15, -10, 15), glm::vec3(2.0f)
    );
    ships.push_back(newsub);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub6.obj",
        litShader,
        new GDTexture("Tex/sub6/diff.png"),
        new GDTexture("Tex/sub6/norm.png"),
        glm::vec3(10, -20, 13), glm::vec3(0.05f), glm::vec3(0, -30, 0)
    );
    ships.push_back(newsub);
    objs.push_back(newsub);

    newsub = new GDObject("3D/sub7.obj",
        litShader,
        new GDTexture("Tex/sub7/diff.png"),
        new GDTexture("Tex/sub7/norm.png"),
        glm::vec3(-20, -10, -12), glm::vec3(3), glm::vec3(0, 40, 0));
    ships.push_back(newsub);
    objs.push_back(newsub);
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

#pragma region Player/Camera Control
        //Camera Control
        CameraControl(deltaTime * 20);
        
        //Player control
        if (activeCamera != mCameraOrtho) {
            TurnControl(deltaTime * 40);
            MovementControl(deltaTime * 0.1f);
        }

        //Torpedo Control
        torpedo_cd -= deltaTime;

        if (glfwGetKey(mWindow->window, GLFW_KEY_SPACE) == GLFW_PRESS)
            queue_torpedo = true;
        if (glfwGetKey(mWindow->window, GLFW_KEY_SPACE) == GLFW_RELEASE && queue_torpedo) {
            queue_torpedo = false;

            if (torpedo_cd <= 0) {
                torpedo_cd = 5.0f;
                spawnTorpedo();
            }
        }

        //Player physics updating
        mPlayer.Velocity -= (mPlayer.Velocity * 0.1f) * (deltaTime * 10);
        mPlayer.Obj->pos += mPlayer.Velocity;
        mPlayer.Obj->pos.y = fmin(mPlayer.Obj->pos.y, 0);

        //Depth updating
        if (mPlayer.lastKnownDepth != (int)mPlayer.Obj->pos.y) {
            mPlayer.lastKnownDepth = (int)mPlayer.Obj->pos.y;
            std::cout << "Current submarine depth: " << mPlayer.lastKnownDepth << std::endl;
        }

        //Player Light Control
        auto playerForward = EulerToVec(cam1stOffset, mPlayer.Obj->rot);
        playerConeLight.pos = mPlayer.Obj->pos + playerForward;
        playerConeLight.dir = playerForward;
        if (glfwGetKey(mWindow->window, GLFW_KEY_F) == GLFW_PRESS)
            queue_lightswitch = true;
        if (glfwGetKey(mWindow->window, GLFW_KEY_F) == GLFW_RELEASE && queue_lightswitch) {
            queue_lightswitch = false;

            playerConeLight.intensity += 20;
            playerConeLight.intensity = (int)playerConeLight.intensity % 60;
        }
#pragma endregion

#pragma region Torpedo updating

        //Loop through all torpedoes
        for (int t_i = 0; t_i < torpedoes.size(); t_i++)
        {
            auto curTorp = torpedoes[t_i];

            //Update age
            curTorp->age += deltaTime;
            //Lerp to target heading
            curTorp->currentHeading += (glm::normalize(curTorp->target - curTorp->Obj->pos) - curTorp->currentHeading) * (deltaTime * 0.9f);
            //Torpedo physics updating
            curTorp->Velocity = curTorp->currentHeading * 17.0f;
            curTorp->Obj->pos += curTorp->Velocity * deltaTime;
            //Torpedo turning
            curTorp->Obj->rot.y = glm::degrees(atan2(curTorp->currentHeading.x, curTorp->currentHeading.z)) + 180;
            
            //Torpedo deleting
            if (curTorp->age > 3) {
                torpedoes.erase(torpedoes.begin() + t_i);
                
                //Delete from main obj list as well
                int objCount = objs.size();
                for (size_t obj_i = 0; obj_i < objCount; obj_i++)
                {
                    if (objs[obj_i] == curTorp->Obj) {
                        objs.erase(objs.begin() + obj_i);
                        break;
                    }
                }

                delete curTorp->Obj;
            }
        }
#pragma endregion

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
            if (activeCamera != mCameraOrtho && !depthWritersOnly) {
                mSkybox->Render(viewMat, activeCamera->proj);
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
                glActiveTexture(GL_TEXTURE1);
                GLuint texdiffAddress = glGetUniformLocation(obj->shader->shaderID, "texdiffuse");
                glBindTexture(GL_TEXTURE_2D, obj->texturediff->texID);
                glUniform1i(texdiffAddress, 1);

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
                glActiveTexture(GL_TEXTURE0);
                GLuint skyboxAddress = glGetUniformLocation(obj->shader->shaderID, "skybox");
                glBindTexture(GL_TEXTURE_2D, mSkybox->textureCubeMap->texID);
                glUniform1i(skyboxAddress, 0);

                //Pass the necessary CPU-computed data to the object shader
                //Transform data
                glm::mat4 tmat = glm::mat4(1.0f);
                tmat = glm::translate(tmat, obj->pos);
                tmat = glm::scale(tmat, obj->scale);
                tmat = glm::rotate(tmat, glm::radians(obj->rot.x), glm::vec3(1, 0, 0));
                tmat = glm::rotate(tmat, glm::radians(obj->rot.y), glm::vec3(0, 1, 0));
                glm::mat4 tmat_VM = tmat;
                glm::mat4 tmat_PVM = activeCamera->proj * viewMat * tmat;
                setMat("transform_model", tmat);
                setMat("transform_projection", tmat_PVM);
                setVec3("cameraPos", activeCamera->cameraPos);

                //Light data
                setVec3("dLightDir", dirLight.dir);
                setVec3("dLightColor", dirLight.color);
                setVec3("cLightPos", playerConeLight.pos);
                setVec3("cLightColor", playerConeLight.color * playerConeLight.intensity);
                setVec3("cLightFaceDir", playerConeLight.dir);
                setVec2("cLightAngle", playerConeLight.angle);
                setFloat("ambientLightIntensity", 0.3f);

                setFloat("specStrength", 2.9f);
                setFloat("specPhong", 7);

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