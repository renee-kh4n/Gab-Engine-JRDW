#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <GD_Graphics/util.h>
#include <GD_Graphics/Shader.h>
#include <GD_Graphics/Texture.h>
#include <GD_Graphics/Window.h>
#include <GD_Graphics/Camera.h>
#include <GD_Graphics/Skybox.h>
#include <GD_Graphics/Light.h>
#include <GD_Graphics/Framebuffer.h>
#include <GD_Graphics/RenderPipeline.h>
#include <GD_Graphics/Mesh.h>

#include <GD_Engine/PhysicsPipeline.h>
#include <GD_Engine/Vector.h>
#include <GD_Engine/Time.h>

using namespace gde;

static Window* mWindow;

int main(void)
{
    auto mTime = new Time();

    /* Initialize GLFW*/
    if (!glfwInit())
        return -1;

    mWindow = new Window(900, 900);
    
    /* Initialize GLAD*/
    gladLoadGL();

#pragma region Rendering Requisites Setup
    //Shaders setup
    auto depthShader = new Shader("Shaders/object.vert", "Shaders/depth.frag");
    auto litShader = new Shader("Shaders/object.vert", "Shaders/lit.frag");
    auto unlitShader = new Shader("Shaders/object.vert", "Shaders/unlit.frag");
    auto Cam3rdPPShader = new Shader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto Cam1stPPShader = new Shader("Shaders/camshader.vert", "Shaders/camshader.frag");
    auto CamOrthoPPShader = new Shader("Shaders/camshader.vert", "Shaders/camshader.frag");

    //Camera setup
    auto mCameraOrtho = new OrthographicCamera(mWindow, CamOrthoPPShader);
    glUseProgram(CamOrthoPPShader->shaderID);
    glUniform1f(glGetUniformLocation(CamOrthoPPShader->shaderID, "saturation"), 1.0f);
    glUniform4fv(glGetUniformLocation(CamOrthoPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1)));
    mCameraOrtho->orthoRange = 450;
    mCameraOrtho->farClip = 1000.0f;
    mCameraOrtho->cameraPos = glm::vec3(0, 0, -50);
    mCameraOrtho->CamF = glm::vec3(0, 0, 1);
    mCameraOrtho->WorldUp = glm::vec3(0, 1, 0);

    //RenderPipeline setup
    auto mRenderPipeline = new RenderPipeline(glm::vec2(mWindow->win_x, mWindow->win_y), mCameraOrtho);
    //PhysicsPipeline setup
    auto mPhysicsPipeline = new PhysicsPipeline();
#pragma endregion

#pragma region Object setup
    //sphere setup
    auto sphere_mesh = new Mesh("3D/sphere.obj");
    auto sphere_material = new Material(unlitShader);
    sphere_material->setOverride<glm::vec3>("color", glm::vec3(1, 0, 0));
    auto sphere_drawcall = new DrawCall(sphere_mesh, sphere_material);
    mRenderPipeline->RegisterDrawCall(sphere_drawcall);

    auto sphere_object = new Object();
    sphere_object->scale = Vector3(1, 1, 1) * 20;
    sphere_object->position.x = 600;
    sphere_object->position.z = 600;
    sphere_object->velocity.x = 50;
    sphere_object->velocity.y = 50;
    sphere_object->velocity.z = 50;
    sphere_object->mDrawCall = sphere_drawcall;
    mPhysicsPipeline->Register(sphere_object);

#pragma endregion

    /// <summary>
    /// MAIN GAME LOOP
    /// </summary>

    while (!glfwWindowShouldClose(mWindow->window))
    {
        //Update pipeline
        mRenderPipeline->RenderFrame();

        //Update window
        glfwSwapBuffers(mWindow->window);
        /* Poll for and process events */
        glfwPollEvents();

        double fixedTickDur = 0;

        if (mTime->TickFixed(&fixedTickDur)) {
            mPhysicsPipeline->Update(fixedTickDur);
        }
    }
    
    mRenderPipeline->CleanUp();

    glfwTerminate();
}