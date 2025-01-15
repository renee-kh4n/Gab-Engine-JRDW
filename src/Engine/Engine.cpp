
#include "Engine.h"

#include "gbe_engine.h"

namespace gbe {
	void Engine::UpdateHandlers()
	{
		mPhysicsHandler = this->current_root->GetHandler<PhysicsHandler>();
		mInputHandler = this->current_root->GetHandler<InputHandler>();
		mLightHandler = this->current_root->GetHandler<ObjectHandler<gbe::LightObject>>();
		mCameraHandler = this->current_root->GetHandler<ObjectHandler<gbe::Camera>>();
		mEarlyUpdate = this->current_root->GetHandler<ObjectHandler<EarlyUpdate>>();
		mUpdate = this->current_root->GetHandler<ObjectHandler<Update>>();
		mLateUpdate = this->current_root->GetHandler<ObjectHandler<LateUpdate>>();
	}
	Engine::Engine()
	{
		this->current_root = nullptr;
		this->queued_rootchange = nullptr;
	}

	bool Engine::ChangeRoot(Root* newroot)
	{
		this->queued_rootchange = newroot;
		return true;
	}

	Root* Engine::CreateBlankRoot()
	{
		auto root_object = new Root();
		root_object->RegisterHandler(new PhysicsHandler(physics::PhysicsPipeline::Get_Instance()));
		root_object->RegisterHandler(new InputHandler());
		root_object->RegisterHandler(new ObjectHandler<gbe::LightObject>());
		root_object->RegisterHandler(new ObjectHandler<gbe::Camera>());
		root_object->RegisterHandler(new ObjectHandler<EarlyUpdate>());
		root_object->RegisterHandler(new ObjectHandler<Update>());
		root_object->RegisterHandler(new ObjectHandler<LateUpdate>());

		return root_object;
	}

	void Engine::Run()
	{
		//WINDOW
		Window* mWindow = new Window(Vector2Int(1280, 720));

#pragma region Rendering Pipeline Setup
		//RenderPipeline setup
		auto mRenderPipeline = new RenderPipeline(mWindow->Get_procaddressfunc(), mWindow->Get_dimentions());
		mRenderPipeline->Init();
#pragma endregion
#pragma region Physics Pipeline Setup
		auto mPhysicsPipeline = new physics::PhysicsPipeline();
		mPhysicsPipeline->Init();
#pragma endregion
#pragma region Asset Loading
		//SHADER CACHING
		auto litShader = new asset::Shader("DefaultAssets/Shaders/lit.shader.gbe");
		auto unlitShader = new asset::Shader("DefaultAssets/Shaders/unlit.shader.gbe");
		auto uiShader = new asset::Shader("DefaultAssets/Shaders/gui.shader.gbe");

		auto CamOrthoPPShader = new asset::Shader("DefaultAssets/Shaders/frame.shader.gbe");
		CamOrthoPPShader->SetOverride("saturation", 1.0f);
		CamOrthoPPShader->SetOverride("tint", Vector4(1, 1, 1, 1));

		mRenderPipeline->SetCameraShader(CamOrthoPPShader);
		//TEXTURE CACHING
		auto ball_tex = new asset::Texture("DefaultAssets/Tex/Maps/Model/basketball.jpg.gbe");

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
		mattex.textureRef.Assign(ball_tex);

		auto create_lit_colored_mat = [litShader, mattex](Vector3 color) {
			auto mat = new Material(litShader);
			mat->textureOverrides.push_back(mattex);
			mat->setOverride("color", color);
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
		auto mGUIPipeline = new gbe::gui::gbuiPipeline(quad_mesh->VAO, uiShader->Get_gl_id());
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
#pragma region Root Loaders
		//forward declared load functions
		std::function<Root*()> create_main_game;
		std::function<Root*()> create_main_menu;

		create_main_menu = [&]() {
			auto game_root = this->CreateBlankRoot();

			//Camera setup
			auto player_input = new InputPlayer(player_name);
			player_input->SetParent(game_root);
			auto camera_parent = new FlyingCameraControl();
			camera_parent->SetParent(player_input);

			//GUI COMMUNICATOR
			auto gui_communicator = new GenericController();
			gui_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([=](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
				if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
					return;

				mGUIPipeline->Click();

				}));
			gui_communicator->SetParent(player_input);

			//SCENE GUI
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
			side_button->Set_onClickAction([=]() {this->ChangeRoot(create_main_game()); });
			side_button->SetParent(button);

			return game_root;
		};

		create_main_game = [&]() {
			auto game_root = this->CreateBlankRoot();

			//Global objects
			//physics force setup
			auto gravity_volume = new ForceVolume();
			gravity_volume->shape = ForceVolume::GLOBAL;
			gravity_volume->mode = ForceVolume::DIRECTIONAL;
			gravity_volume->vector = Vector3(0.f, -10, 0.f);
			gravity_volume->forceMode = ForceVolume::VELOCITY;
			gravity_volume->SetParent(game_root);

			//light
			auto directional_light = new DirectionalLight();
			directional_light->Set_Color(Vector3(1, 1, 1));
			directional_light->Set_Intensity(1);
			directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(70, 0, 0)));
			directional_light->SetParent(game_root);
			directional_light->Set_ShadowmapResolutions(1080);

			//Camera setup
			auto player_input = new InputPlayer(player_name);
			player_input->SetParent(game_root);
			auto camera_parent = new FlyingCameraControl();
			camera_parent->SetParent(player_input);

			auto mPerspectiveCam = new PerspectiveCamera(mWindow);
			mPerspectiveCam->angles = 55;
			mPerspectiveCam->farClip = 40.0f;
			mPerspectiveCam->WorldUp = Vector3(0, 1, 0);
			mPerspectiveCam->SetParent(camera_parent);

			camera_parent->TranslateWorld(Vector3(0, 0, -10));

			//GUI COMMUNICATOR
			auto gui_communicator = new GenericController();
			gui_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([=](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
				if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
					return;

				mGUIPipeline->Click();

				}));
			gui_communicator->SetParent(player_input);

			//Actual objects
			//platform
			RigidObject* platform = new RigidObject(true);
			platform->SetParent(game_root);
			platform->Local().position.Set(Vector3(0, -5, 0));
			platform->Local().rotation.Set(Quaternion::Euler(Vector3(0, 0, 0)));
			platform->Local().scale.Set(Vector3(10, 1, 10));
			BoxCollider* platform_collider = new BoxCollider();
			platform_collider->SetParent(platform);
			platform_collider->Local().position.Set(Vector3(0, 0, 0));
			RenderObject* platform_renderer = new RenderObject(cube_drawcall);
			platform_renderer->SetParent(platform_collider);

			//Balls
			auto spawnball = [get_random_drawcall, game_root](Vector3 pos, float radius) {
				RigidObject* ball = new RigidObject();
				ball->SetParent(game_root);

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
			auto gridsize = Vector3(1, 0, 1);
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

			//SCENE GUI
			gbe::gui::gb_canvas* main_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
			mGUIPipeline->SetActiveCanvas(main_canvas);

			gbe::gui::gb_button* button = new gbe::gui::gb_button();
			button->bl_pivot = Vector2(-1, -1);
			button->tr_pivot = button->bl_pivot;
			button->bl_offset = Vector2(150, 50);
			button->tr_offset = button->bl_offset + Vector2(300, 100);
			button->normal_color = Vector4(0, 0, 1, 1);
			button->hovered_color = Vector4(1, 0, 0, 1);
			button->Set_onClickAction([=]() {this->ChangeRoot(create_main_menu()); });
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

			return game_root;
		};
#pragma endregion

		auto initial_root = create_main_menu();
		this->current_root = initial_root;
		this->UpdateHandlers();

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
			mInputSystem->UpdateStates([this](std::string name, gbe::input::InputAction* action, bool changed) {
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

			if (this->mCameraHandler->object_list.size() > 0) {
				auto current_camera = this->mCameraHandler->object_list.front();
				Matrix4 frustrum = current_camera->getproj() * current_camera->GetViewMat();
				mRenderPipeline->RenderFrame(current_camera->World().position.Get(), current_camera->World().GetForward(), frustrum, current_camera->nearClip, current_camera->farClip);
			}

			mGUIPipeline->DrawActiveCanvas();
			//Update the window
			mWindow->SwapBuffers();

			//Update other handlers
			auto onTick = [=](double deltatime) {
				physics::PhysicsPipeline::Get_Instance()->Tick(deltatime);
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

			//Queued root change
			if (this->queued_rootchange != nullptr) {
				if (this->current_root != nullptr)
				{
					this->current_root->Destroy();
				}
			}

			//Delete all queued for deletions
			std::list<Object*> toDelete;

			this->current_root->CallRecursively([&toDelete](Object* object) {
				if (object->get_isDestroyed()) {
					toDelete.push_back(object);
				}
				});

			for (auto deletee : toDelete)
			{
				delete deletee;
			}

			if (this->queued_rootchange != nullptr) {
				//Object handlers setup
				this->current_root = this->queued_rootchange;
				this->queued_rootchange = nullptr;

				this->UpdateHandlers();
			}
		}
#pragma endregion
		mRenderPipeline->CleanUp();
		mWindow->Terminate();
	}
}