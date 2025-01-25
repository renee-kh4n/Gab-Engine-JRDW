
#include "Engine.h"

#include "gbe_engine.h"

namespace gbe {
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
		root_object->RegisterHandler(new ObjectHandler<gbe::LightObject>());
		root_object->RegisterHandler(new ObjectHandler<gbe::Camera>());
		root_object->RegisterHandler(new ObjectHandler<PhysicsUpdate>());
		root_object->RegisterHandler(new ObjectHandler<InputPlayer>());
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
		mPhysicsPipeline->Set_OnFixedUpdate_callback(
			[=](float physicsdeltatime) {
				auto phandler = this->current_root->GetHandler<PhysicsUpdate>();
				for (auto updatable : phandler->object_list)
				{
					updatable->InvokePhysicsUpdate(physicsdeltatime);
				}
			}
		);
#pragma endregion
#pragma region Audio Pipeline Setup
		auto mAudioPipeline = new audio::AudioPipeline();
		mAudioPipeline->Init();
#pragma endregion
#pragma region Asset Loading
		//AUDIO CACHING
		auto audio_ui_hover = new asset::Audio("DefaultAssets/Audio/bubble hover.aud.gbe");
		auto audio_ui_click = new asset::Audio("DefaultAssets/Audio/uiclick.aud.gbe");
		auto audio_move = new asset::Audio("DefaultAssets/Audio/bubble movement.aud.gbe");
		auto audio_shoot = new asset::Audio("DefaultAssets/Audio/bubble shoot.aud.gbe");
		auto audio_hairdryer = new asset::Audio("DefaultAssets/Audio/hair dryer.aud.gbe");
		auto audio_soap = new asset::Audio("DefaultAssets/Audio/soap.aud.gbe");
		auto audio_jump = new asset::Audio("DefaultAssets/Audio/quack jump.aud.gbe");
		auto audio_toothpaste = new asset::Audio("DefaultAssets/Audio/toothpaste.aud.gbe");
		auto audio_15secrem = new asset::Audio("DefaultAssets/Audio/15secrem.aud.gbe");

		//SHADER CACHING
		auto litShader = new asset::Shader("DefaultAssets/Shaders/lit.shader.gbe");
		auto bubbleShader = new asset::Shader("DefaultAssets/Shaders/bubble.shader.gbe");
		auto unlitShader = new asset::Shader("DefaultAssets/Shaders/unlit.shader.gbe");
		auto uiShader = new asset::Shader("DefaultAssets/Shaders/gui.shader.gbe");

		auto CamOrthoPPShader = new asset::Shader("DefaultAssets/Shaders/frame.shader.gbe");
		CamOrthoPPShader->SetOverride("saturation", 1.0f);
		CamOrthoPPShader->SetOverride("tint", Vector4(1, 1, 1, 1));

		mRenderPipeline->SetCameraShader(CamOrthoPPShader);

		//TEXTURE CACHING
		//UI
		auto startbubble_tex = new asset::Texture("DefaultAssets/Tex/UI/startbubble.img.gbe");
		auto exitbubble_tex = new asset::Texture("DefaultAssets/Tex/UI/exitbubble.img.gbe");
		auto mainmenu_tex = new asset::Texture("DefaultAssets/Tex/UI/mainmenu.img.gbe");
		//MODEL
		auto ball_tex = new asset::Texture("DefaultAssets/Tex/Maps/Model/basketball.img.gbe");

		//MESH CACHING
		auto quad_mesh = new Mesh("DefaultAssets/3D/plane.obj");
		auto sphere_mesh = new Mesh("DefaultAssets/3D/sphere.obj");
		auto cube_mesh = new Mesh("DefaultAssets/3D/cube.obj");
		auto duck_mesh = new Mesh("DefaultAssets/3D/Rubber Ducky.obj");

		//MATERIAL CACHING
		auto unlit_white_mat = new Material(unlitShader);
		unlit_white_mat->setOverride("color", Vector4(1, 1, 1, 1));

		auto lit_mat_trans = new Material(litShader);
		lit_mat_trans->setOverride("color", Vector4(0, 1, 1, 0.5f));
		lit_mat_trans->setOverride("transparency", true);

		auto lit_white_mat = new Material(litShader);
		lit_white_mat->setOverride("color", Vector4(1, 1, 1, 1));
		lit_white_mat->setOverride("ambientLightTint", Vector3(1, 1, 1) * 0.2f);
		lit_white_mat->setOverride<bool>("hasDiffuseTex", false);
		lit_white_mat->setOverride<float>("specStrength", 0.5f);
		lit_white_mat->setOverride<float>("specPhong", 16);

		auto mattex = MaterialTexture();
		mattex.parameterName = "texdiffuse";
		mattex.textureRef.Assign(ball_tex);

		auto create_coloredbubble_mat = [=](Vector3 color) {
			auto mat = new Material(bubbleShader);
			mat->textureOverrides.push_back(mattex);
			mat->setOverride("transparency", true);
			mat->setOverride("color", Vector4(color.x, color.y, color.z, 0.3f));
			mat->setOverride("hasDiffuseTex", false);
			//mat->setOverride("specStrength", 0.2f);
			//mat->setOverride("specPhong", 1);

			return mat;
			};

		std::vector<Material*> lit_colored_mats = {
			create_coloredbubble_mat(Vector3(1, 0, 0)),
			create_coloredbubble_mat(Vector3(0, 1, 0)),
			create_coloredbubble_mat(Vector3(0, 0, 1)),
			create_coloredbubble_mat(Vector3(1, 1, 0)),
			create_coloredbubble_mat(Vector3(1, 0, 1)),
			create_coloredbubble_mat(Vector3(0, 1, 1))
		};

		//DRAWCALL CACHING
		auto line_drawcall = new DrawCall(quad_mesh, unlit_white_mat);
		mRenderPipeline->RegisterDrawCall(line_drawcall);

		auto whiteball_drawcall = new DrawCall(sphere_mesh, lit_white_mat);
		mRenderPipeline->RegisterDrawCall(whiteball_drawcall);

		auto transparent = new DrawCall(sphere_mesh, lit_mat_trans, 1);
		mRenderPipeline->RegisterDrawCall(transparent);

		std::vector<DrawCall*> particle_drawcalls;

		for (auto mat : lit_colored_mats)
		{
			auto new_drawcall = new DrawCall(sphere_mesh, mat, 1);
			mRenderPipeline->RegisterDrawCall(new_drawcall);
			particle_drawcalls.push_back(new_drawcall);
		}

		const auto get_random_drawcall = [particle_drawcalls]() {
			return particle_drawcalls[rand() % particle_drawcalls.size()];
			};

		auto cube_drawcall = new DrawCall(cube_mesh, lit_white_mat);
		mRenderPipeline->RegisterDrawCall(cube_drawcall);

		auto duck_drawcall = new DrawCall(duck_mesh, lit_white_mat);
		mRenderPipeline->RegisterDrawCall(duck_drawcall);
#pragma endregion
#pragma region GUI Pipeline Setup
		auto mGUIPipeline = new gbe::gui::gbuiPipeline(quad_mesh->VAO, mRenderPipeline->Get_mainbufferId(), uiShader);
		mGUIPipeline->Set_target_resolution(mWindow->Get_dimentions());
#pragma endregion
#pragma region Input
		auto mInputSystem = new InputSystem();
		auto player_name = "MAIN";
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::MOUSE_LEFT>());
		mInputSystem->RegisterActionListener(player_name, new WasdDeltaImplementation());
		mInputSystem->RegisterActionListener(player_name, new MouseScrollImplementation());
		mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_MIDDLE>());
		mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_RIGHT>());
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::SPACE>());
#pragma endregion
#pragma region Util functions
		auto create_image_button = [=](asset::Texture* tex, gbe::gui::gb_canvas* main_canvas, Vector2 percent_pos, Vector2 half_extents, std::function<void()> onpress) {
			gbe::gui::gb_image* start_image = new gbe::gui::gb_image();
			start_image->Set_Image(tex);
			start_image->bl_pivot = percent_pos;
			start_image->bl_offset = -half_extents;
			start_image->tr_pivot = start_image->bl_pivot;
			start_image->tr_offset = -start_image->bl_offset;
			main_canvas->AddRootChild(start_image);

			gbe::gui::gb_button* button = new gbe::gui::gb_button(start_image);
			button->bl_pivot = percent_pos;
			button->bl_offset = -half_extents;
			button->tr_pivot = button->bl_pivot;
			button->tr_offset = -button->bl_offset;
			button->normal_color = Vector4(1, 1, 1, 0);
			button->hovered_color = Vector4(1, 1, 1, 1);
			main_canvas->AddRootChild(button);
			button->Set_onClickAction([=]() {
				onpress();
				audio_ui_click->Play();
				});
			};
#pragma endregion
#pragma region Root Loaders
		//forward declared load functions
		std::function<Root* ()> create_main_game;
		std::function<Root* ()> create_main_menu;
		std::function<Root* ()> create_success_screen;

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

			gbe::gui::gb_image* main_image = new gbe::gui::gb_image();
			main_image->Set_Image(mainmenu_tex);
			main_image->bl_pivot = Vector2(-1, -1);
			main_image->tr_pivot = Vector2(1, 1);
			main_image->bl_offset = Vector2(0, 0);
			main_image->tr_offset = Vector2(0, 0);
			main_image->Set_handleType(gui::gb_rect::PointerEventHandleType::PASS);
			main_canvas->AddRootChild(main_image);

			create_image_button(startbubble_tex, main_canvas, 
				Vector2(-0.6, -0.6),
				Vector2(70, 50),
				[=]() {
				this->ChangeRoot(create_main_game());
				});

			return game_root;
			};

		create_main_game = [&]() {
			auto game_root = this->CreateBlankRoot();

#pragma region one n only objects
			//forward declaration
			auto player = new RigidObject();

			//Spawn funcs

			//Balls
			auto spawnball = [get_random_drawcall, game_root](Vector3 pos, float radius, bool collides = true) {
				RigidObject* ball = new RigidObject();
				ball->SetParent(game_root);

				if (collides) {
					auto cradle_collider = new SphereCollider();
					cradle_collider->SetParent(ball);
				}

				auto sphere_renderobject = new RenderObject(get_random_drawcall());
				sphere_renderobject->SetParent(ball);

				ball->Local().position.Set(pos);
				ball->Local().scale.Set(radius);

				return ball;
				};
			//Generic Particle system
			auto create_particle = [=](ParticleSystem* creator) {
				float randf = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randsize = 0.1f + (0.6f * randf);

				return spawnball(Vector3::zero, randsize, false);
				};

			//Hairdryer
			auto create_spray = [=](Vector3 pos) {
				auto hairspray = new Object();
				hairspray->Local().position.Set(pos);
				hairspray->SetParent(game_root);
				TriggerRigidObject* hairspray_trigger = new TriggerRigidObject();
				hairspray_trigger->Local().scale.Set(Vector3(4, 20, 4));
				hairspray_trigger->SetParent(hairspray);
				//RenderObject* hairspray_trigger_renderer = new RenderObject(cube_drawcall);
				//hairspray_trigger_renderer->SetParent(hairspray_trigger);
				//hairspray_trigger_renderer->Local().position.Set(Vector3::zero);
				BoxCollider* hairspray_trigger_collider = new BoxCollider();
				hairspray_trigger_collider->SetParent(hairspray_trigger);
				hairspray_trigger_collider->Local().position.Set(Vector3(0, 0, 0));
				hairspray_trigger->Set_OnStay([=](gbe::PhysicsObject* other, float delta) {
					if (other == player) {
						player->GetRigidbody()->AddForce((physics::PhysicsVector3)(Vector3(0, 4000, 0) * delta));
					}
					});
				hairspray_trigger->Set_OnExit([=](gbe::PhysicsObject* other) {
					if (other == player) {
						std::cout << "exitted spray." << std::endl;
					}
					});
				hairspray_trigger->Set_OnEnter([=](gbe::PhysicsObject* other) {
					if (other == player) {
						std::cout << "enetered spray." << std::endl;
					}
					});
				auto hairspray_particle_system = new ParticleSystem(create_particle);
				hairspray_particle_system->SetParent(hairspray);
				auto bl_corner_bound = Vector3(-2, -1, -2);
				hairspray_particle_system->SetBounds(bl_corner_bound, -bl_corner_bound);
				hairspray_particle_system->Set_force(Vector3(0, 600, 0));
				hairspray_particle_system->Set_rate(10);
				};
			auto create_platform = [=](Vector3 pos, Vector3 scale) {
				RigidObject* platform = new RigidObject(true);
				platform->SetParent(game_root);
				platform->Local().position.Set(pos);
				platform->Local().rotation.Set(Quaternion::Euler(Vector3(0, 0, 0)));
				platform->Local().scale.Set(scale);
				BoxCollider* platform_collider = new BoxCollider();
				platform_collider->SetParent(platform);
				platform_collider->Local().position.Set(Vector3(0, 0, 0));
				RenderObject* platform_renderer = new RenderObject(cube_drawcall);
				platform_renderer->SetParent(platform_collider);
				};

			//Global objects
			//physics force setup
			auto gravity_volume = new ForceVolume();
			gravity_volume->shape = ForceVolume::GLOBAL;
			gravity_volume->mode = ForceVolume::DIRECTIONAL;
			gravity_volume->vector = Vector3(0.f, -12, 0.f);
			gravity_volume->forceMode = ForceVolume::VELOCITY;
			gravity_volume->SetParent(game_root);

			//light
			auto directional_light = new DirectionalLight();
			directional_light->Set_Color(Vector3(1, 1, 1));
			directional_light->Set_Intensity(1);
			directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(70, 0, 0)));
			directional_light->SetParent(game_root);
			directional_light->Set_ShadowmapResolutions(2160);

			//Player and Camera setup
			auto f_speed = 200.0f;
			auto f_jump = 320.0f;

			PerspectiveCamera* player_cam = new PerspectiveCamera(mWindow);

			player->SetParent(game_root);
			auto player_renderer = new RenderObject(get_random_drawcall());
			player_renderer->SetParent(player);
			auto player_collider = new SphereCollider();
			player_collider->SetParent(player);
			auto player_particle_system = new ParticleSystem(create_particle);
			player_particle_system->SetParent(player);
			auto bl_corner_bound = Vector3(-0.5, -0.5, -0.5);
			player_particle_system->SetBounds(bl_corner_bound, -bl_corner_bound);
			player_particle_system->Set_force(Vector3(0, 200, 0));
			player_particle_system->Set_rate(20);
			player_particle_system->Set_enabled(false);

			//duck inside
			auto duck_object = new GenericObject([=](GenericObject* self, float delta) {
				self->SetWorldPosition(player->World().position.Get());
				});
			duck_object->SetParent(game_root);
			auto duck_renderer = new RenderObject(duck_drawcall);
			duck_renderer->Local().position.Set(Vector3(0, -0.7f, 0));
			duck_renderer->Local().scale.Set(Vector3(1) * 0.6f);
			duck_renderer->SetParent(duck_object);

			auto camera_parent = new GenericObject([=](GenericObject* self, float delta) {
				self->SetWorldPosition(player->World().position.Get());
			});
			camera_parent->SetParent(game_root);
			player_cam->angles = 80;
			player_cam->farClip = 200.0f;
			player_cam->WorldUp = Vector3(0, 1, 0);
			player_cam->SetParent(camera_parent);
			player_cam->TranslateWorld(Vector3(0, 3, -10));

			//ground check
			auto groundcheck = [=]() {
				auto floorcheck = physics::Raycast(player->World().position.Get(), Vector3(0, -1.1f, 0));
				return floorcheck.result;
				};
			//BALL SHOOTER
			auto shoot_func = [spawnball, camera_parent]() {
				auto spawnpos = camera_parent->World().position.Get() - (camera_parent->World().GetUp() * 0.3f);

				auto ball = spawnball(spawnpos, 0.2f);
				ball->GetRigidbody()->AddForce((physics::PhysicsVector3)(camera_parent->World().GetForward() * 1000.0f));
				};

			//Player Controller Logic
			auto player_input = new InputPlayer(player_name);
			player_input->SetParent(game_root);
			auto camera_controller = new TPCameraController();
			camera_controller->Set_pivot(camera_parent);
			camera_controller->SetParent(player_input);
			auto input_communicator = new GenericController();
			//Left click customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([=](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
				if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
					return;

				mGUIPipeline->Click();

				}));
			//WASD customer
			input_communicator->AddCustomer(new InputCustomer<WasdDelta>([=](WasdDelta* value, bool changed) {
				if (value->state == WasdDelta::END || !groundcheck())
					player_particle_system->Set_enabled(false);
				if(value->state == WasdDelta::WHILE && groundcheck())
					player_particle_system->Set_enabled(true);

				if (value->state != WasdDelta::WHILE)
					return;



				duck_renderer->World().rotation.Set(player_cam->World().rotation.Get());

				Vector3 right_vec = player_cam->World().GetRight() * f_speed;
				auto forward_vec = right_vec.Cross(Vector3(0, 1, 0)).Normalize() * f_speed;

				if (value->delta.y > 0)
					player->GetRigidbody()->AddContinuousForce((physics::PhysicsVector3)(forward_vec));
				if (value->delta.y < 0)
					player->GetRigidbody()->AddContinuousForce((physics::PhysicsVector3)(-forward_vec));
				if (value->delta.x > 0)
					player->GetRigidbody()->AddContinuousForce((physics::PhysicsVector3)(-right_vec));
				if (value->delta.x < 0)
					player->GetRigidbody()->AddContinuousForce((physics::PhysicsVector3)(right_vec));

				}));
			//Spacebar customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::SPACE>>([=](KeyPress<Keys::SPACE>* value, bool changed) {
				if (value->state != KeyPress<Keys::SPACE>::START)
					return;

				if (groundcheck()) { //raycast ground here
					auto curvel = player->GetRigidbody()->Get_velocity();
					curvel.y = 0;
					player->GetRigidbody()->Set_velocity(curvel);
					player->GetRigidbody()->AddForce((physics::PhysicsVector3)(Vector3(0, 1, 0) * f_jump));
				}

				}));
			input_communicator->SetParent(player_input);


			//Death Trigger
			auto death_checker = new GenericObject([=](GenericObject* self, float delta) {
				if (player->World().position.Get().y < -20) {
					player->SetWorldPosition(Vector3::zero);
					player->GetRigidbody()->Set_velocity(Vector3::zero);
				}
				});
			death_checker->SetParent(game_root);
#pragma endregion
			
			//Actual objects
			
			//platform
			create_platform(Vector3(0, -10, 0), Vector3(10, 1, 10));
			create_spray(Vector3(0, -20, 20));
			create_platform(Vector3(0, -2, 35), Vector3(10, 1, 10));
			create_platform(Vector3(30, -6, 26), Vector3(10, 1, 3));
			create_spray(Vector3(40, -20, 0));
			create_spray(Vector3(40, -15, -10));
			create_spray(Vector3(40, -10, -20));
			create_platform(Vector3(40, 5, -35), Vector3(5, 1, 5));

			//Goal
			TriggerRigidObject* goal_trigger = new TriggerRigidObject();
			goal_trigger->TranslateWorld(Vector3(Vector3(40, 8, -35)));
			goal_trigger->Local().scale.Set(Vector3(3, 3, 3));
			goal_trigger->SetParent(game_root);
			RenderObject* goal_trigger_renderer = new RenderObject(duck_drawcall);
			goal_trigger_renderer->SetParent(goal_trigger);
			goal_trigger_renderer->Local().position.Set(Vector3::zero);
			BoxCollider* goal_trigger_collider = new BoxCollider();
			goal_trigger_collider->SetParent(goal_trigger);
			goal_trigger_collider->Local().position.Set(Vector3(0, 0, 0));
			goal_trigger->Set_OnStay([=](gbe::PhysicsObject* other, float delta) {
				if (other == player) {
					this->ChangeRoot(create_main_menu());
				}
			});


			//SCENE GUI
			gbe::gui::gb_canvas* main_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
			mGUIPipeline->SetActiveCanvas(main_canvas);

			create_image_button(exitbubble_tex, main_canvas,
				Vector2(-0.6, -0.6),
				Vector2(70, 50),
				[=]() {
					this->ChangeRoot(create_main_menu());
				});

			return game_root;
			};

		create_success_screen = [&]() {
			return nullptr;
			};
#pragma endregion

		auto initial_root = create_main_menu();
		this->current_root = initial_root;

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
			auto inputhandler = this->current_root->GetHandler<InputPlayer>();

			mInputSystem->UpdateStates([=](std::string name, gbe::input::InputAction* action, bool changed) {
				for (auto input_player : inputhandler->object_list) {
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
			for (auto updatable : this->current_root->GetHandler<EarlyUpdate>()->object_list)
			{
				updatable->InvokeEarlyUpdate();
			}

			//Update Render pipeline
			for (auto light : this->current_root->GetHandler<LightObject>()->object_list)
			{
				if (mRenderPipeline->TryPushLight(light->GetData(), false) == false) {
					break;
				}
			}

			auto pos = Vector3::zero;
			auto forward = Vector3::zero;
			auto frustrum = Matrix4();
			auto nearclip = 0.0f;
			auto farclip = 0.0f;

			if (this->current_root->GetHandler<Camera>()->object_list.size() > 0) {
				auto current_camera = this->current_root->GetHandler<Camera>()->object_list.front();
				pos = current_camera->World().position.Get();
				forward = current_camera->World().GetForward();
				frustrum = current_camera->getproj() * current_camera->GetViewMat();
				nearclip = current_camera->nearClip;
				farclip = current_camera->farClip;
			}
			mRenderPipeline->RenderFrame(pos, forward, frustrum, nearclip, farclip);
			mGUIPipeline->DrawActiveCanvas();

			//Update the window
			mWindow->SwapBuffers();

			//Update other handlers
			auto physicshandler_generic = this->current_root->GetHandler<PhysicsObject>();
			auto physicshandler = static_cast<PhysicsHandler*>(physicshandler_generic);
			auto updatehandler = this->current_root->GetHandler<Update>();
			auto lateupdatehandler = this->current_root->GetHandler<LateUpdate>();

			auto onTick = [=](double deltatime) {
				physics::PhysicsPipeline::Get_Instance()->Tick(deltatime);
				physicshandler->Update();

				float delta_f = (float)deltatime;

				//Normal Update
				for (auto updatable : updatehandler->object_list)
				{
					updatable->InvokeUpdate(delta_f);
				}
				//Late Update
				for (auto updatable : lateupdatehandler->object_list)
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
			}
		}
#pragma endregion
		mRenderPipeline->CleanUp();
		mWindow->Terminate();
	}
}