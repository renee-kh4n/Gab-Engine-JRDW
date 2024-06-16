#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <GD_Graphics/util.h>
#include <GD_Graphics/Shader.h>
#include <GD_Graphics/Texture.h>
#include <GD_Graphics/Window.h>
#include <GD_Graphics/Skybox.h>
#include <GD_Graphics/Light.h>
#include <GD_Graphics/Framebuffer.h>
#include <GD_Graphics/RenderPipeline.h>
#include <GD_Graphics/Mesh.h>

#include <GD_Engine/Input/InputSystem.h>
#include <GD_Engine/ObjectHandlers/ObjectHandlers.h>
#include <GD_Engine/Objects/Objects.h>
#include <GD_Engine/ObjectFunctions/ObjectFunctions.h>
#include <GD_Engine/Datatypes/Vectors.h>
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
    glUseProgram(CamOrthoPPShader->shaderID);
    glUniform1f(glGetUniformLocation(CamOrthoPPShader->shaderID, "saturation"), 1.0f);
    glUniform4fv(glGetUniformLocation(CamOrthoPPShader->shaderID, "tint"), 1, glm::value_ptr(glm::vec4(1, 1, 1, 1)));

    //RenderPipeline setup
    Camera* active_camera = nullptr;
    auto mRenderPipeline = new RenderPipeline(glm::vec2(mWindow->win_x, mWindow->win_y));
#pragma endregion

#pragma region Input
    auto mInputSystem = new InputSystem();
#pragma endregion

#pragma region Object setup
    //Object handlers setup
    auto mPhysicsHandler = new PhysicsHandler();
    auto mInputHandler = new InputHandler();
    auto mEarlyUpdate = new ObjectHandler<EarlyUpdate>();
    auto mUpdate = new ObjectHandler<Update>();
    auto mLateUpdate = new ObjectHandler<LateUpdate>();
    //root
    auto root_object = new Root();
    root_object->RegisterHandler(mPhysicsHandler);
    root_object->RegisterHandler(mInputHandler);
    root_object->RegisterHandler(mEarlyUpdate);
    root_object->RegisterHandler(mUpdate);
    root_object->RegisterHandler(mLateUpdate);

    //Camera setup
    auto camera_dolly = new OrbitalControl();
    camera_dolly->SetParent(root_object);

    auto mCameraOrtho = new OrthographicCamera(mWindow, CamOrthoPPShader);
    mCameraOrtho->orthoRange = 30;
    mCameraOrtho->farClip = 1000.0f;
    mCameraOrtho->CamF = glm::vec3(0, 0, 1);
    mCameraOrtho->WorldUp = glm::vec3(0, 1, 0);
    mCameraOrtho->Translate(glm::vec3(0, 0, -50));
    mCameraOrtho->SetParent(camera_dolly);
    active_camera = mCameraOrtho;

    //Mesh and material caching
    auto sphere_mesh = new Mesh("3D/sphere.obj");
    auto sphere_material = new Material(unlitShader);
    sphere_material->setOverride<glm::vec3>("color", glm::vec3(1, 0, 0));
    auto sphere_drawcall = new DrawCall(sphere_mesh, sphere_material);
    mRenderPipeline->RegisterDrawCall(sphere_drawcall);

    auto CreateParticleFunction = [sphere_drawcall, root_object, unlitShader, mRenderPipeline]() {
        //sphere rigidobject setup
        auto sphere_rigidobject = new RigidObject();
        sphere_rigidobject->damping = 1.0f;
        sphere_rigidobject->SetParent(root_object);

        //sphere renderobject setup
        auto sphere_renderobject = new RenderObject(sphere_drawcall);
        sphere_renderobject->Scale(Vector3(1, 1, 1) * 0.2f);
        sphere_renderobject->SetParent(sphere_rigidobject);

        return sphere_rigidobject;
    };

    //particle system setup
    auto system = new ParticleSystem(CreateParticleFunction);
    system->spawns_per_sec = 200;
    system->start_force.random_between_two = true;
    system->start_force.valueA = Vector3(-1, 1, -1) * 200;
    system->start_force.valueB = Vector3(1, 5, 1) * 200;
    system->start_lifetime.valueA = 2;
    system->SetParent(root_object);

    //physics force setup
    auto gravity_volume = new ForceVolume();
    gravity_volume->shape = ForceVolume::GLOBAL;
    gravity_volume->mode = ForceVolume::DIRECTIONAL;
    gravity_volume->vector = Vector3(0, -10, 0);
    gravity_volume->forceMode = ForceVolume::VELOCITY;
    gravity_volume->SetParent(root_object);

#pragma endregion

    /// <summary>
    /// MAIN GAME LOOP
    /// </summary>

    while (!glfwWindowShouldClose(mWindow->window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        //Early update
        for (auto updatable : mEarlyUpdate->object_list)
        {
            updatable->InvokeEarlyUpdate();
        }

        //Update pipeline
        mRenderPipeline->RenderFrame(active_camera->World()->position, active_camera->GetViewMat(), active_camera->getproj(), active_camera->mShader);

        //Update window
        glfwSwapBuffers(mWindow->window);

        mTime->TickFixed([mPhysicsHandler, mUpdate, mLateUpdate, root_object](double deltatime) {
            mPhysicsHandler->Update(deltatime);

            //Normal Update
            for (auto updatable : mUpdate->object_list)
            {
                updatable->InvokeUpdate(deltatime);
            }
            //Late Update
            for (auto updatable : mLateUpdate->object_list)
            {
                updatable->InvokeLateUpdate(deltatime);
            }
        });

        std::list<Object*> toDelete;

        root_object->CallRecursively([&toDelete](Object* object) {
            if (object->get_isDestroyed()) {
                toDelete.push_back(object);
            }
        });

        for (auto deletee : toDelete)
        {
            delete deletee;
        }
    }
    
    mRenderPipeline->CleanUp();

    glfwTerminate();
}