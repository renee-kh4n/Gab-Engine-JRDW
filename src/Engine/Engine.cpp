
#include "Engine.h"

namespace gbe {
    
    Engine::Engine()
	{

	}

	void Engine::Run()
	{
        auto mTime = new Time();

        Window* mWindow = new Window();

#pragma region Rendering Pipeline Setup
        //RenderPipeline setup
        Camera* active_camera = nullptr;
        auto mRenderPipeline = new RenderPipeline(mWindow->Get_procaddressfunc(), glm::vec2(mWindow->Get_win_x(), mWindow->Get_win_y()));
        
        //Shaders setup
        auto depthShader = new Shader("DefaultAssets/Shaders/object.vert", "DefaultAssets/Shaders/depth.frag");
        auto litShader = new Shader("DefaultAssets/Shaders/object.vert", "DefaultAssets/Shaders/lit.frag");
        auto unlitShader = new Shader("DefaultAssets/Shaders/object.vert", "DefaultAssets/Shaders/unlit.frag");
        auto Cam3rdPPShader = new Shader("DefaultAssets/Shaders/camshader.vert", "DefaultAssets/Shaders/camshader.frag");
        auto Cam1stPPShader = new Shader("DefaultAssets/Shaders/camshader.vert", "DefaultAssets/Shaders/camshader.frag");

        auto CamOrthoPPShader = new Shader("DefaultAssets/Shaders/camshader.vert", "DefaultAssets/Shaders/camshader.frag");
        CamOrthoPPShader->SetOverride("saturation", 1.0f);
        CamOrthoPPShader->SetOverride("tint", Vector4(1, 1, 1, 1));

        //Reassign to Pipeline
        mRenderPipeline->Set_DepthShader(depthShader);

        auto mSkybox = new gbe::rendering::Skybox(new gbe::rendering::TextureCubeMap("DefaultAssets/Tex/Skybox/rainbow", ".png"));
        mSkybox->shader = new Shader("DefaultAssets/Shaders/skybox.vert", "DefaultAssets/Shaders/skybox.frag");
        mRenderPipeline->SetSkybox(mSkybox);
#pragma endregion
#pragma region Physics Pipeline Setup
        auto mPhysicsPipeline = new physics::PhysicsPipeline();
        mPhysicsPipeline->Init();
#pragma endregion

#pragma region Input
        auto mInputSystem = new InputSystem();
        auto player_name = "MAIN";
        mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::MOUSE_LEFT>());
        mInputSystem->RegisterActionListener(player_name, new MouseScrollImplementation());
        mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_MIDDLE>());
        mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_RIGHT>());
#pragma endregion

#pragma region Scene Root setup
        //Object handlers setup
        auto mPhysicsHandler = new PhysicsHandler();
        mPhysicsHandler->SetPipeline(mPhysicsPipeline);

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
#pragma endregion
#pragma region Scene Objects setup
        //Global objects
        //physics force setup
        auto gravity_volume = new ForceVolume();
        gravity_volume->shape = ForceVolume::GLOBAL;
        gravity_volume->mode = ForceVolume::DIRECTIONAL;
        gravity_volume->vector = Vector3(0.f, -4, 0.f);
        gravity_volume->forceMode = ForceVolume::VELOCITY;
        gravity_volume->SetParent(root_object);

        //light
        auto directional_light = new DirectionalLight();
        directional_light->Set_Color(Vector3(1, 1, 1));
        directional_light->Set_Intensity(1);
        directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(70, 0, 0)));
        directional_light->SetParent(root_object);

        //Camera setup
        auto player_input = new InputPlayer(player_name);
        player_input->SetParent(root_object);
        auto camera_parent = new FlyingCameraControl();
        camera_parent->SetParent(player_input);

        auto mPerspectiveCam = new PerspectiveCamera(mWindow, CamOrthoPPShader);
        mPerspectiveCam->angles = 60;
        mPerspectiveCam->farClip = 50.0f;
        mPerspectiveCam->WorldUp = Vector3(0, 1, 0);
        mPerspectiveCam->SetParent(camera_parent);
        
        active_camera = mPerspectiveCam;

        camera_parent->TranslateWorld(Vector3(0, 0, -10));

        //TEXTURE CACHING
        auto chewbacca_tex = new Texture("DefaultAssets/Tex/cubeacca.jpg");

        //MESH CACHING
        auto plane_mesh = new Mesh("DefaultAssets/3D/plane.obj");
        auto sphere_mesh = new Mesh("DefaultAssets/3D/sphere.obj");
        auto cube_mesh = new Mesh("DefaultAssets/3D/cube.obj");

        //MATERIAL CACHING
        auto unlit_white_mat = new Material(unlitShader);
        unlit_white_mat->setOverride("color", Vector3(1, 1, 1));

        auto lit_white_mat = new Material(litShader);
        lit_white_mat->setOverride("color", Vector3(1, 1, 1));
        lit_white_mat->setOverride<bool>("hasDiffuseTex", false);
        lit_white_mat->setOverride<float>("specStrength", 0.5f);
        lit_white_mat->setOverride<float>("specPhong", 16);

        auto mattex = MaterialTexture();
        mattex.parameterName = "texdiffuse";
        mattex.texture = chewbacca_tex;

        auto create_lit_colored_mat = [litShader, mattex](Vector3 color) {
            auto mat = new Material(litShader);
            mat->textureOverrides.push_back(mattex);
            mat->setOverride("color", color * 0.9f);
            mat->setOverride("hasDiffuseTex", true);
            mat->setOverride("specStrength", 0.5f);
            mat->setOverride("specPhong", 16);

            return mat;
        };

        std::vector<Material*> lit_colored_mats = {
            create_lit_colored_mat(Vector3(1, 0, 0)),
            create_lit_colored_mat(Vector3(0, 1, 0)),
            create_lit_colored_mat(Vector3(0, 0, 1)),
            create_lit_colored_mat(Vector3(1, 1, 0)),
            create_lit_colored_mat(Vector3(1, 0, 1)),
            create_lit_colored_mat(Vector3(0, 1, 1))
        };

        //DRAWCALL CACHING
        auto line_drawcall = new DrawCall(plane_mesh, unlit_white_mat);
        mRenderPipeline->RegisterDrawCall(line_drawcall);

        auto whiteball_drawcall = new DrawCall(sphere_mesh, lit_white_mat);
        mRenderPipeline->RegisterDrawCall(whiteball_drawcall);
        std::vector<DrawCall*> particle_drawcalls;

        for (auto mat : lit_colored_mats)
        {
            auto new_drawcall = new DrawCall(sphere_mesh, mat);
            mRenderPipeline->RegisterDrawCall(new_drawcall);
            particle_drawcalls.push_back(new_drawcall);
        }

        auto AddParticleChild = [whiteball_drawcall, root_object, unlitShader, mRenderPipeline](Object* something) {
            auto sphere_renderobject = new RenderObject(whiteball_drawcall);
            sphere_renderobject->SetParent(something);
        };

        auto cube_drawcall = new DrawCall(cube_mesh, lit_white_mat);
        mRenderPipeline->RegisterDrawCall(cube_drawcall);

        //Actual objects

        //platform
        
        RigidObject* platform = new RigidObject(true);
        platform->SetParent(root_object);
        platform->Local().position.Set(Vector3(0, -5, 0));
        platform->Local().rotation.Set(Quaternion::Euler(Vector3(0, 40, 0)));
        platform->Local().scale.Set(Vector3(4, 1, 3));
        BoxCollider* platform_collider = new BoxCollider();
        platform_collider->SetParent(platform);
        platform_collider->Local().position.Set(Vector3(0, 0, 0));
        RenderObject* platform_renderer = new RenderObject(cube_drawcall);
        platform_renderer->SetParent(platform_collider);
        
        //Balls
        auto spawnball = [AddParticleChild, root_object](Vector3 pos, float radius) {
            RigidObject* ball = new RigidObject();
            ball->SetParent(root_object);

            auto cradle_collider = new SphereCollider();
            cradle_collider->SetParent(ball);

            AddParticleChild(ball);

            ball->Local().position.Set(pos);
            ball->Local().scale.Set(radius);

            return ball;
        };

        auto radius = 0.3f;
        auto startpos = Vector3(0, 0, 0);
        auto gridsize = Vector3(10, 0, 10);
        for (int x = 0; x < gridsize.x; x++)
            for (int z = 0; z < gridsize.z; z++)
            {
                auto finalpos = startpos + (Vector3(x, 0, z) * radius);

                spawnball(finalpos, radius);

                /*
                auto cradle_chain = new ChainJoint(15);
                cradle_chain->to_rbody = cradle_ball;
                cradle_chain->SetParent(root_object);

                auto cradle_line = new LineRenderer(line_drawcall, camera_parent, cradle_ball, cradle_chain);
                cradle_line->SetThickness(0.02f);
                cradle_line->SetParent(root_object);
                */
            }

        //BALL SHOOTER
        auto shooter = new GenericController();
        shooter->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([spawnball, camera_parent](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
            if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
                return;

            auto spawnpos = camera_parent->World().position.Get() - (camera_parent->World().GetUp() * 0.3f);

            auto ball = spawnball(spawnpos, 0.2f);
            ball->body.AddForce((physics::PhysicsVector3)(camera_parent->World().GetForward() * 1000.0f));
        }));
        shooter->SetParent(player_input);


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
            mRenderPipeline->SetView(active_camera->World().position.Get(), active_camera->GetViewMat(), active_camera->getproj());
            mRenderPipeline->SetPostProcessing(active_camera->mShader);
            mRenderPipeline->RenderFrame();
            //Render window
            mWindow->SwapBuffers();

            //Update other handlers
            auto onTick = [mPhysicsPipeline, mPhysicsHandler, mUpdate, mLateUpdate, root_object](double deltatime) {
                mPhysicsPipeline->Tick(deltatime);
                mPhysicsHandler->Update();

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