
#include "Engine.h"

namespace gbe {
    
    Engine::Engine()
	{

	}

	void Engine::Run()
	{
        auto mTime = new Time();

        Window* mWindow = new Window();

#pragma region Rendering Requisites Setup
        //RenderPipeline setup
        Camera* active_camera = nullptr;
        auto mRenderPipeline = new RenderPipeline(mWindow->Get_procaddressfunc(), glm::vec2(mWindow->Get_win_x(), mWindow->Get_win_y()));
        
        //Shaders setup
        auto depthShader = new Shader("Assets/Shaders/object.vert", "Assets/Shaders/depth.frag");
        auto litShader = new Shader("Assets/Shaders/object.vert", "Assets/Shaders/lit.frag");
        auto unlitShader = new Shader("Assets/Shaders/object.vert", "Assets/Shaders/unlit.frag");
        auto Cam3rdPPShader = new Shader("Assets/Shaders/camshader.vert", "Assets/Shaders/camshader.frag");
        auto Cam1stPPShader = new Shader("Assets/Shaders/camshader.vert", "Assets/Shaders/camshader.frag");

        auto CamOrthoPPShader = new Shader("Assets/Shaders/camshader.vert", "Assets/Shaders/camshader.frag");
        CamOrthoPPShader->SetOverride("saturation", 1.0f);
        CamOrthoPPShader->SetOverride("tint", glm::vec4(1, 1, 1, 1));

        //auto mGUIPipeline = new GUIPipeline(mWindow->GetNativeHandle());
#pragma endregion

#pragma region Input
        auto mInputSystem = new InputSystem();
        auto player_name = "MAIN";
        mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::MOUSE_LEFT>());
        mInputSystem->RegisterActionListener(player_name, new MouseScrollImplementation());
        mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_MIDDLE>());
        mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_RIGHT>());
#pragma endregion

#pragma region Object setup
        //Object handlers setup
        auto mPhysicsHandler = new PhysicsHandler();
        auto mInputHandler = new InputHandler();
        auto mLightHandler = new ObjectHandler<gbe::LightObject>();
        auto mEarlyUpdate = new ObjectHandler<EarlyUpdate>();
        auto mUpdate = new ObjectHandler<Update>();
        auto mLateUpdate = new ObjectHandler<LateUpdate>();
        //root
        auto root_object = new Root();
        root_object->RegisterHandler(mPhysicsHandler);
        root_object->RegisterHandler(mInputHandler);
        root_object->RegisterHandler(mLightHandler);
        root_object->RegisterHandler(mEarlyUpdate);
        root_object->RegisterHandler(mUpdate);
        root_object->RegisterHandler(mLateUpdate);

        //Camera setup
        auto player_input = new InputPlayer(player_name);
        player_input->SetParent(root_object);
        auto camera_parent = new FlyingCameraControl();
        camera_parent->SetParent(player_input);

        auto mPerspectiveCam = new PerspectiveCamera(mWindow, CamOrthoPPShader);
        mPerspectiveCam->angles = 60;
        mPerspectiveCam->farClip = 1500.0f;
        mPerspectiveCam->WorldUp = Vector3(0, 1, 0);
        mPerspectiveCam->SetParent(camera_parent);
        
        active_camera = mPerspectiveCam;

        camera_parent->TranslateWorld(Vector3(0, 0, -10));

        //Mesh and material caching

        //unlit line rendercall
        auto mat = new Material(unlitShader);
        mat->setOverride<glm::vec3>("color", glm::vec3(1, 1, 1));
        auto lineDrawCall = new DrawCall(new Mesh("Assets/3D/plane.obj"), mat);
        mRenderPipeline->RegisterDrawCall(lineDrawCall);

        //particle rendercall
        auto sphere_mesh = new Mesh("Assets/3D/sphere.obj");
        auto sphere_tex = new Texture("Assets/Tex/cubeacca.jpg");

        auto create_rendercall = [sphere_tex, sphere_mesh, litShader, unlitShader, mRenderPipeline](Vector3 color) {
            auto mattex = MaterialTexture();
            mattex.parameterName = "texdiffuse";
            mattex.texture = sphere_tex;

            auto mat = new Material(litShader);
            mat->textureOverrides.push_back(mattex);
            mat->setOverride<glm::vec3>("color", color * 0.9f);
            mat->setOverride<bool>("hasDiffuseTex", true);
            mat->setOverride<float>("specStrength", 0.5f);
            mat->setOverride<float>("specPhong", 16);
            auto new_drawcall = new DrawCall(sphere_mesh, mat);
            mRenderPipeline->RegisterDrawCall(new_drawcall);
            return new_drawcall;
            };

        std::vector<DrawCall*> drawcalls = {
            create_rendercall(Vector3(1, 0, 0)),
            create_rendercall(Vector3(0, 1, 0)),
            create_rendercall(Vector3(0, 0, 1)),
            create_rendercall(Vector3(1, 1, 0)),
            create_rendercall(Vector3(1, 0, 1)),
            create_rendercall(Vector3(0, 1, 1)),
        };
        //reference sphere renderobject setup

        auto CreateParticleFunction = [drawcalls, root_object, unlitShader, mRenderPipeline]() {
            //sphere rigidobject setup
            auto sphere_rigidobject = new RigidObject();
            sphere_rigidobject->damping = 1.0f;
            sphere_rigidobject->mass = 10;

            //sphere renderobject setup
            auto sphere_renderobject = new RenderObject(drawcalls[rand() % drawcalls.size()]);
            sphere_renderobject->TranslateLocal(Vector3(0, 0, 0));
            sphere_renderobject->SetParent(sphere_rigidobject);

            return sphere_rigidobject;
            };

        //Collision testing

        auto createswing = [camera_parent, lineDrawCall, CreateParticleFunction, root_object](Vector3 position, float length, float radius, Vector3 offset = Vector3::zero, RigidObject** out_ball = nullptr) {
            auto ball = CreateParticleFunction();
            ball->TranslateLocal(position + offset);
            ball->SetParent(root_object);
            ball->velocity = Vector3(0, 0, 0);
            ball->SetScale(Vector3(1, 1, 1) * (radius));

            //Sphere collider
            auto collider = new Collider(radius);
            collider->SetParent(ball);

            auto chain = new ChainJoint(length);
            chain->TranslateLocal(position);
            chain->to_rbody = ball;
            chain->SetParent(root_object);

            auto line = new LineRenderer(lineDrawCall, camera_parent, ball, chain);
            line->SetParent(root_object);

            if (out_ball != nullptr) {
                *out_ball = ball;
            }

            return chain;
            };

        auto length = 20.0f;
        auto radius = 5.0f;
        auto grav_str = -10;
        auto count = 8;

        auto cradle_radius = 20.0f;

        auto startheight = length / 2.0f;
        auto spinspeed = 10.0f;
        RigidObject* cradle_ball = nullptr;
        auto cradle_pivot = createswing(Vector3(0, startheight, 0), 0.1f, 1.0f, Vector3::zero, &cradle_ball);
        cradle_ball->amgularDamp = 0.6f;

        for (size_t i = 0; i < count; i++)
        {
            auto rad = ((float)i / count) * 360 * ((float)3.14 / 180.0f);
            auto startpos = Vector3(cosf(rad), 0, sinf(rad)) * cradle_radius;
            startpos.y = startheight;

            auto chain = createswing(startpos, length, radius, Vector3::zero);
            chain->SetParent(cradle_ball);
        }

        //physics force setup
        auto gravity_volume = new ForceVolume();
        gravity_volume->shape = ForceVolume::GLOBAL;
        gravity_volume->mode = ForceVolume::DIRECTIONAL;
        gravity_volume->vector = Vector3(0.f, grav_str, 0.f);
        gravity_volume->forceMode = ForceVolume::VELOCITY;
        gravity_volume->SetParent(root_object);

        //light
        auto directional_light = new DirectionalLight();
        directional_light->Set_Color(Vector3(1, 1, 1));
        directional_light->Set_Intensity(1);
        directional_light->Orient(Vector3(0, 0, -1), Vector3(1, 0, 0));
        directional_light->SetParent(root_object);

#pragma endregion

        /// MAIN GAME LOOP
        while (!mWindow->ShouldClose())
        {
            /* Poll for and process events */
            mWindow->UpdateState();

            //Update input system
            mInputSystem->UpdateStates([mInputHandler](std::string name, gbe::input::InputAction* action, bool changed) {
                for (auto input_player : mInputHandler->object_list) {
                    if (input_player->get_player_name() != name)
                        continue;

                    for (auto controller : input_player->controllers.object_list)
                        controller->ForEach_inputreceivers([action, changed](InputCustomer_base* input_customer) {
                        input_customer->TryReceive(action, changed);
                            });
                }
                }, mWindow);

            //Early update
            for (auto updatable : mEarlyUpdate->object_list)
            {
                updatable->InvokeEarlyUpdate();
            }

            //Update Render pipeline
            for (auto light : mLightHandler->object_list)
            {
                if (mRenderPipeline->TryPushLight(light->GetData(), false) == false) {
                    break;
                }
            }
            mRenderPipeline->SetView(active_camera->World()->position, active_camera->GetViewMat(), active_camera->getproj());
            mRenderPipeline->SetPostProcessing(active_camera->mShader);
            mRenderPipeline->RenderFrame();
            //Render window
            mWindow->SwapBuffers();

            //Update other handlers
            auto onTick = [mPhysicsHandler, mUpdate, mLateUpdate, root_object](double deltatime) {
                mPhysicsHandler->Update(deltatime);

                float delta_f = (float)deltatime;

                //Normal Update
                for (auto updatable : mUpdate->object_list)
                {
                    updatable->InvokeUpdate(delta_f);
                }
                //Late Update
                for (auto updatable : mLateUpdate->object_list)
                {
                    updatable->InvokeLateUpdate(delta_f);
                }
            };
            mTime->TickFixed(onTick);

            mInputSystem->ResetStates(mWindow);

            //Delete all queued for deletions
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

        mWindow->Terminate();
	}
}