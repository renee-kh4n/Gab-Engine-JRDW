
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
		auto mRenderPipeline = new RenderPipeline(mWindow, mWindow->Get_dimentions());
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
		//mAudioPipeline->Init();
#pragma endregion
#pragma region Editor Setup
		auto mEditor = new gbe::Editor(mRenderPipeline, mWindow);
		mWindow->AddAdditionalEventProcessor([mEditor](void* newevent) {
			mEditor->ProcessRawWindowEvent(newevent);
			});
#pragma endregion
#pragma region Asset Loading
		//AUDIO CACHING
		//auto audio_ui_hover = new asset::Audio("DefaultAssets/Audio/bubble hover.aud.gbe");
		//auto audio_ui_click = new asset::Audio("DefaultAssets/Audio/uiclick.aud.gbe");


		//MESH CACHING
		auto test_mesh = new asset::Mesh("DefaultAssets/3D/test.obj.gbe");
		auto cube_mesh = new asset::Mesh("DefaultAssets/3D/cube.obj.gbe");
		
		//SHADER CACHING
		auto unlitShader = new asset::Shader("DefaultAssets/Shaders/unlit.shader.gbe");
		auto idShader = new asset::Shader("DefaultAssets/Shaders/id.shader.gbe");
		auto gridShader = new asset::Shader("DefaultAssets/Shaders/grid.shader.gbe");

		//TEXTURE CACHING
		auto test_tex = new asset::Texture("DefaultAssets/Tex/Maps/Model/test.img.gbe");
		
		//MATERIAL CACHING
		auto id_mat = new asset::Material("DefaultAssets/Materials/id.mat.gbe");
		auto test_mat = new asset::Material("DefaultAssets/Materials/unlit.mat.gbe");
		test_mat->setOverride("color", Vector4(Vector3(1, 2, 1).Normalize(), 1.0f));
		test_mat->setOverride("colortex", test_tex);
		auto cube_mat = new asset::Material("DefaultAssets/Materials/grid.mat.gbe");

		//DRAW CALL CACHING
		auto test_drawcall = mRenderPipeline->RegisterDrawCall(test_mesh, test_mat);
		auto cube_drawcall = mRenderPipeline->RegisterDrawCall(cube_mesh, cube_mat);

#pragma endregion
#pragma region GUI Pipeline Setup
		//auto mGUIPipeline = new gbe::gui::gbuiPipeline(quad_mesh->VAO, mRenderPipeline->Get_mainbufferId(), uiShader);
		//mGUIPipeline->Set_target_resolution(mWindow->Get_dimentions());
#pragma endregion
#pragma region Input
		auto mInputSystem = new InputSystem();
		auto player_name = "MAIN";
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::MOUSE_LEFT>());
		mInputSystem->RegisterActionListener(player_name, new WasdDeltaImplementation());
		mInputSystem->RegisterActionListener(player_name, new MouseScrollImplementation());
		mInputSystem->RegisterActionListener(player_name, new MouseDeltaImplementation());
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::SPACE>());
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::ESCAPE>());
		mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_RIGHT>());
		mInputSystem->RegisterActionListener(player_name, new MouseDragImplementation<Keys::MOUSE_MIDDLE>());
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
			button->normal_color = Vector4(0, 0, 0, 0);
			button->hovered_color = Vector4(1, 1, 1, 1);
			main_canvas->AddRootChild(button);
			button->Set_onClickAction([=]() {
				onpress();
				//audio_ui_click->Play();
				});
			button->Set_onHoverAction([=]() {
				//audio_ui_hover->Play();
				});
			};
#pragma endregion
#pragma region Root Loaders
		//forward declared load functions
		std::function<Root* ()> create_main_game;

		create_main_game = [&]() {
			auto game_root = this->CreateBlankRoot();
			//SCENE GUI
			gbe::gui::gb_canvas* maingame_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
			//mGUIPipeline->SetActiveCanvas(maingame_canvas);

#pragma region scene singletons
			//forward declaration
			auto player = new RigidObject();

			//Spawn funcs

			auto create_test = [=](Vector3 pos, Vector3 scale, Vector3 renderscale) {
				RigidObject* test = new RigidObject();
				test->SetParent(game_root);
				test->Local().position.Set(pos);
				test->Local().rotation.Set(Quaternion::Euler(Vector3(0, 0, 0)));
				test->Local().scale.Set(scale);
				BoxCollider* platform_collider = new BoxCollider();
				platform_collider->SetParent(test);
				platform_collider->Local().position.Set(Vector3(0, 0, 0));
				RenderObject* platform_renderer = new RenderObject(test_drawcall);
				platform_renderer->SetParent(platform_collider);
				platform_renderer->Local().scale.Set(renderscale);

				return test;
				};

			auto create_box = [=](Vector3 pos, Vector3 scale) {
				RigidObject* test = new RigidObject(true);
				test->SetParent(game_root);
				test->Local().position.Set(pos);
				test->Local().rotation.Set(Quaternion::Euler(Vector3(0, 0, 0)));
				test->Local().scale.Set(scale);
				BoxCollider* platform_collider = new BoxCollider();
				platform_collider->SetParent(test);
				platform_collider->Local().position.Set(Vector3(0, 0, 0));
				RenderObject* platform_renderer = new RenderObject(cube_drawcall);
				platform_renderer->SetParent(platform_collider);

				return test;
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
			directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(80, 90, 0)));
			directional_light->SetParent(game_root);
			directional_light->Set_ShadowmapResolutions(2160);

			//Player and Camera setup
			auto f_speed = 100.0f;
			auto f_jump = 180.0f;

			auto player_input = new InputPlayer(player_name);
			player_input->SetParent(game_root);
			auto camera_controller = new FlyingCameraControl();
			camera_controller->SetParent(player_input);

			PerspectiveCamera* player_cam = new PerspectiveCamera(mWindow);
			player_cam->SetParent(camera_controller);

			//ground check
			auto groundcheck = [=]() {
				auto floorcheck = physics::Raycast(player->World().position.Get(), Vector3(0, -1.5f, 0));
				return floorcheck.result;
				};

			//Player Controller Logic
			auto input_communicator = new GenericController();
			input_communicator->SetParent(player_input);
			//Left click customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([=](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
				if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
					return;

				//mGUIPipeline->Click();
				}));
			//WASD customer
			input_communicator->AddCustomer(new InputCustomer<WasdDelta>([=](WasdDelta* value, bool changed) {

				}));
			//Spacebar customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::SPACE>>([=](KeyPress<Keys::SPACE>* value, bool changed) {
				if (value->state != KeyPress<Keys::SPACE>::START)
					return;
				}));
			//ESCAPE Customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::ESCAPE>>([=](KeyPress<Keys::ESCAPE>* value, bool changed) {
				if (value->state != KeyPress<Keys::ESCAPE>::START)
					return;

				}));
#pragma endregion

#pragma region scene objects
			Vector3 from(-20, 0, 0);
			Vector3 to(100, 0, 0);
			Vector3 up(0, 1, 0);
			float height = 20;
			float pillarInterval = 13;
			float pillarThickness = 2.5f;
			float wallThickness = 0.5f;
			float beamInterval = 9;
			float beamThickness = 1.8f;
			float windowHeight = 3;
			Vector3 windowSize(2, 4, 0.6f);
			float roofHeight = 2.0f;
			float roofThickness = 3.0f;

			//PCG FUNCTION

			//STEP 1: CALCULATE VECTORS
			Vector3 delta = to - from;
			Vector3 stepdir = delta.Normalize();
			Vector3 forward = stepdir.Cross(up).Normalize();
			float abdistance = delta.Magnitude();
			float halfheight = height * 0.5f;

			//STEP 2: EXECUTE MAIN WALL SEGMENT LOOP
			for (float x = 0; x < abdistance; x += pillarInterval)
			{
				//STEP 2.1: PILLAR PLACEMENT
				Vector3 pillarpos = stepdir * x;
				pillarpos += up * halfheight;
				Vector3 pillarscale = Vector3(0.5f) * pillarThickness;
				pillarscale.y = halfheight;

				create_box(from + pillarpos, pillarscale);

				//STEP 2.2: WALL SEGMENT PLACEMENT
				Vector3 wallpos = stepdir * (x + (pillarInterval * 0.5f));
				wallpos += up * halfheight;
				Vector3 wallscale = Vector3(pillarInterval * 0.5f, halfheight, wallThickness * 0.5f);

				create_box(from + wallpos, wallscale);

				//STEP 2.3 BEAM PLACEMENT
				for (float y = beamInterval; y < height; y+= beamInterval)
				{
					Vector3 beampos = stepdir * (x + (pillarInterval * 0.5f));
					beampos += up * y;
					Vector3 beamscale = Vector3(pillarInterval * 0.5f, beamThickness * 0.5f, beamThickness * 0.5f);

					create_box(from + beampos, beamscale);
				}

				//STEP 2.4 WINDOW PLACEMENT
				for (float y = 0; y + beamInterval < height; y += beamInterval)
				{
					Vector3 windowpos = stepdir * (x + (pillarInterval * 0.5f));
					windowpos += up * (y + windowHeight + (windowSize.y * 0.5f));
					windowpos += -forward * ((wallThickness * 0.5f) + (windowSize.z * 0.5f));
					Vector3 windowscale = windowSize * 0.5f;

					create_box(from + windowpos, windowscale);
				}

				//STEP 2.5 ROOF PLACEMENT
				Vector3 roofpos = stepdir * (x + (pillarInterval * 0.5f));
				roofpos += up * (height + (roofHeight * 0.5f));
				Vector3 roofscale = Vector3(pillarInterval * 0.5f, roofHeight * 0.5f, roofThickness * 0.5f);

				create_box(from + roofpos, roofscale);
			}

#pragma endregion

			return game_root;
			};

#pragma endregion
		auto initial_root = create_main_game();
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
					//mGUIPipeline->Set_target_resolution(newdimensions);
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
			//mGUIPipeline->PassScreenSpaceMousePos(bl_pivoted_mousepos);

			//Early update
			for (auto updatable : this->current_root->GetHandler<EarlyUpdate>()->object_list)
			{
				updatable->InvokeEarlyUpdate();
			}

			//Update Render pipeline
			//EDITOR UPDATE
			mEditor->PrepareFrame();
			mEditor->DrawFrame();
			//<----------MORE EDITOR FUNCTIONS GO HERE
			mEditor->PresentFrame();
			//ENGINE UPDATE
			for (auto light : this->current_root->GetHandler<LightObject>()->object_list)
			{
				if (mRenderPipeline->TryPushLight(light->GetData(), false) == false) {
					break;
				}
			}

			auto pos = Vector3::zero;
			auto forward = Vector3::zero;
			auto frustrum = Matrix4();
			auto viewm = Matrix4();
			auto projm = Matrix4();
			auto nearclip = 0.0f;
			auto farclip = 0.0f;

			if (this->current_root->GetHandler<Camera>()->object_list.size() > 0) {
				auto current_camera = this->current_root->GetHandler<Camera>()->object_list.front();
				pos = current_camera->World().position.Get();
				forward = current_camera->World().GetForward();
				frustrum = current_camera->getproj() * current_camera->GetViewMat();
				viewm = current_camera->GetViewMat();
				projm = current_camera->getproj();
				nearclip = current_camera->nearClip;
				farclip = current_camera->farClip;
			}
			mRenderPipeline->RenderFrame(viewm, projm, nearclip, farclip);
			//mGUIPipeline->DrawActiveCanvas();

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
				//mGUIPipeline->Update(deltatime);
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