
#include "Engine.h"

namespace gbe {
    
    Engine::Engine()
	{

	}

	void Engine::Run()
	{
        //WINDOW
        Window* mWindow = new Window(Vector2Int(1280, 720));

#pragma region Rendering Pipeline Setup
        //RenderPipeline setup
        Camera* active_camera = nullptr;
        auto mRenderPipeline = new RenderPipeline(mWindow->Get_procaddressfunc(), mWindow->Get_dimentions());
#pragma endregion
#pragma region Physics Pipeline Setup
        auto mPhysicsPipeline = new physics::PhysicsPipeline();
        mPhysicsPipeline->Init();
#pragma endregion
#pragma region Asset Loader Assigning
        asset::TextureLoader::Set_load_func(gfx::GraphicsAssetLoader::Load_Texture);
#pragma endregion
#pragma region Asset Loading
        //SHADER CACHING
        auto litShader = new Shader("DefaultAssets/Shaders/lit.vert", "DefaultAssets/Shaders/lit.frag");
        auto unlitShader = new Shader("DefaultAssets/Shaders/lit.vert", "DefaultAssets/Shaders/unlit.frag");
        auto Cam3rdPPShader = new Shader("DefaultAssets/Shaders/frame.vert", "DefaultAssets/Shaders/frame.frag");
        auto Cam1stPPShader = new Shader("DefaultAssets/Shaders/frame.vert", "DefaultAssets/Shaders/frame.frag");
        auto uiShader = new Shader("DefaultAssets/Shaders/gui.vert", "DefaultAssets/Shaders/gui.frag");

        auto CamOrthoPPShader = new Shader("DefaultAssets/Shaders/frame.vert", "DefaultAssets/Shaders/frame.frag");
        CamOrthoPPShader->SetOverride("saturation", 1.0f);
        CamOrthoPPShader->SetOverride("tint", Vector4(1, 1, 1, 1));
        //TEXTURE CACHING
        auto chewbacca_tex = new asset::Texture("spheretex", "DefaultAssets/Tex/Maps/Model/cubeacca.jpg");

        //MESH CACHING
        auto quad_mesh = new Mesh("DefaultAssets/3D/plane.obj");
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
        auto line_drawcall = new DrawCall(quad_mesh, unlit_white_mat);
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

        const auto get_random_drawcall = [particle_drawcalls]() {
            return particle_drawcalls[rand() % particle_drawcalls.size()];
        };

        auto cube_drawcall = new DrawCall(cube_mesh, lit_white_mat);
        mRenderPipeline->RegisterDrawCall(cube_drawcall);
#pragma endregion
#pragma region GUI Pipeline Setup
        auto mGUIPipeline = new gbe::gui::gbuiPipeline(quad_mesh->VAO, uiShader->shaderID);
        mGUIPipeline->Set_target_resolution(mWindow->Get_dimentions());
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
        directional_light->Set_Color(Vector3(0, 1, 1));
        directional_light->Set_Intensity(1);
        directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(70, 0, 0)));
        directional_light->SetParent(root_object);
        directional_light->Set_ShadowmapResolutions(1080);

        //Camera setup
        auto player_input = new InputPlayer(player_name);
        player_input->SetParent(root_object);
        auto camera_parent = new FlyingCameraControl();
        camera_parent->SetParent(player_input);

        auto mPerspectiveCam = new PerspectiveCamera(mWindow, CamOrthoPPShader);
        mPerspectiveCam->angles = 55;
        mPerspectiveCam->farClip = 40.0f;
        mPerspectiveCam->WorldUp = Vector3(0, 1, 0);
        mPerspectiveCam->SetParent(camera_parent);
        
        active_camera = mPerspectiveCam;

        camera_parent->TranslateWorld(Vector3(0, 0, -10));

        //GUI COMMUNICATOR
        auto gui_communicator = new GenericController();
        gui_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([mGUIPipeline](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
            if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
                return;

            mGUIPipeline->Click();
            
            }));
        gui_communicator->SetParent(player_input);

        //Actual objects
        //platform
        RigidObject* platform = new RigidObject(true);
        platform->SetParent(root_object);
        platform->Local().position.Set(Vector3(0, -5, 0));
        platform->Local().rotation.Set(Quaternion::Euler(Vector3(0, 0, 0)));
        platform->Local().scale.Set(Vector3(10, 1, 10));
        BoxCollider* platform_collider = new BoxCollider();
        platform_collider->SetParent(platform);
        platform_collider->Local().position.Set(Vector3(0, 0, 0));
        RenderObject* platform_renderer = new RenderObject(cube_drawcall);
        platform_renderer->SetParent(platform_collider);
        
        //Balls
        auto spawnball = [get_random_drawcall, root_object](Vector3 pos, float radius) {
            RigidObject* ball = new RigidObject();
            ball->SetParent(root_object);

            auto cradle_collider = new SphereCollider();
            cradle_collider->SetParent(ball);

            auto sphere_renderobject = new RenderObject(get_random_drawcall());
            sphere_renderobject->SetParent(ball);

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
            }

        //BALL SHOOTER
        auto shoot_func = [spawnball, camera_parent]() {
            auto spawnpos = camera_parent->World().position.Get() - (camera_parent->World().GetUp() * 0.3f);

            auto ball = spawnball(spawnpos, 0.2f);
            ball->body.AddForce((physics::PhysicsVector3)(camera_parent->World().GetForward() * 1000.0f));
        };
#pragma endregion
#pragma region GUI OBJECTS SETUP
        gbe::gui::gb_canvas* main_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
        mGUIPipeline->SetActiveCanvas(main_canvas);

        gbe::gui::gb_button* button = new gbe::gui::gb_button();
        button->bl_pivot = Vector2(-1, -1);
        button->tr_pivot = button->bl_pivot;
        button->bl_offset = Vector2(150, 50);
        button->tr_offset = button->bl_offset + Vector2(300, 100);
        button->normal_color = Vector4(0, 0, 1, 1);
        button->hovered_color = Vector4(1, 0, 0, 1);
        main_canvas->AddRootChild(button);
        gbe::gui::gb_button* side_button = new gbe::gui::gb_button();
        side_button->normal_color = Vector4(0, 1, 0, 1);
        side_button->hovered_color = Vector4(1, 0, 0, 1);
        side_button->bl_pivot = Vector2(-1, -1);
        side_button->tr_pivot = Vector2(-1, 1);
        side_button->tr_offset = Vector2(-10, 0);
        side_button->bl_offset = Vector2(-110, 0);
        side_button->Set_onClickAction(shoot_func);
        side_button->SetParent(button);

#pragma endregion
#pragma region MAIN LOOP
        //GLOBAL RUNTIME COMPONENTS
        auto mTime = new Time();

        /// MAIN GAME LOOP
        while (!mWindow->ShouldClose())
        {
            /* Poll for and process events */
            mWindow->UpdateState();
            gbe::window::WindowEventType windoweventtype;
            while (mWindow->PollEvents(windoweventtype))
            {
                if (windoweventtype == gbe::window::WindowEventType::RESIZE) {
                    auto newdimensions = mWindow->Get_dimentions();
                    mGUIPipeline->Set_target_resolution(newdimensions);
                    mRenderPipeline->SetResolution(newdimensions);
                }
            }

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
            //Update GUI system
            auto bl_pivoted_mousepos = mWindow->GetMousePos();
            bl_pivoted_mousepos.y = mWindow->Get_dimentions().y - bl_pivoted_mousepos.y;
            mGUIPipeline->PassScreenSpaceMousePos(bl_pivoted_mousepos);

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
            mRenderPipeline->SetPostProcessing(active_camera->mShader);
            Matrix4 frustrum = active_camera->getproj() * active_camera->GetViewMat();
            mRenderPipeline->RenderFrame(active_camera->World().position.Get(), active_camera->World().GetForward(), frustrum, active_camera->nearClip, active_camera->farClip);
            mGUIPipeline->DrawActiveCanvas();
            //Update the window
            mWindow->SwapBuffers();

            //Update other handlers
            auto onTick = [mWindow, mGUIPipeline, mPhysicsPipeline, mPhysicsHandler, mUpdate, mLateUpdate, root_object](double deltatime) {
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

                //GUI
                mGUIPipeline->Update(deltatime);
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
#pragma endregion
        mRenderPipeline->CleanUp();
        mWindow->Terminate();
	}
}