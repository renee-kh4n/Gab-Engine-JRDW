#include "Engine.h"
#include "gbe_engine.h"

#include "Editor/gbe_editor.h"
#include "Graphics/gbe_graphics.h"
#include "GUI/gbe_gui.h"
#include "Math/gbe_math.h"
#include "Physics/gbe_physics.h"
#include "Window/gbe_window.h"
#include "Audio/gbe_audio.h"
#include "Asset/gbe_asset.h"

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

	Root* Engine::GetCurrentRoot()
	{
		return this->current_root;
	}

	void Engine::Run()
	{
		//WINDOW
		Window* mWindow = new Window(Vector2Int(1280, 720));

#pragma region Rendering Pipeline Setup
		//RenderPipeline setup
		auto mRenderPipeline = new RenderPipeline(mWindow, mWindow->Get_dimentions());
#pragma endregion
		//GLOBAL RUNTIME COMPONENTS
		auto mTime = new Time();
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
		auto mEditor = new gbe::Editor(mRenderPipeline, mWindow, this, mTime);
		mWindow->AddAdditionalEventProcessor([mEditor](void* newevent) {
			mEditor->ProcessRawWindowEvent(newevent);
			});
#pragma endregion
#pragma region Asset Loading
		//AUDIO CACHING

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
		//test_mat->setOverride("color", Vector4(Vector3(1, 2, 1).Normalize(), 1.0f));
		test_mat->setOverride("colortex", test_tex);
		auto cube_mat = new asset::Material("DefaultAssets/Materials/grid.mat.gbe");

		//DRAW CALL CACHING
		auto test_drawcall = mRenderPipeline->RegisterDrawCall(test_mesh, test_mat);
		auto cube_drawcall = mRenderPipeline->RegisterDefaultDrawCall(cube_mesh, cube_mat);

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
#pragma region Root Loaders
		//forward declared load functions
		std::function<Root* ()> create_main_game;

		create_main_game = [&]() {
			auto game_root = this->CreateBlankRoot();

#pragma region scene singletons
			//forward declaration
			auto player = new RigidObject();

			//Spawn funcs

			auto create_test = [&](Vector3 pos, Vector3 scale, Vector3 renderscale) {
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

			auto create_box = [&](Vector3 pos, Vector3 scale, Quaternion rotation = Quaternion::Euler(Vector3(0,0,0))) {
				RigidObject* test = new RigidObject(true);
				test->SetParent(game_root);
				test->Local().position.Set(pos);
				test->Local().rotation.Set(rotation);
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

			//================INPUT HANDLING================//
			auto input_communicator = new GenericController();
			input_communicator->SetParent(player_input);
			//Left click customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::MOUSE_LEFT>>([&](KeyPress<Keys::MOUSE_LEFT>* value, bool changed) {
				if (value->state != KeyPress<Keys::MOUSE_LEFT>::START)
					return;

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
			
			//CALL THE BUILDER
			ext::AnimoBuilder::GenerationParams params{};
			auto builder_result = ext::AnimoBuilder::AnimoBuilder::Generate(params);

			//BUILD THE RESULT
			for (auto& objdata : builder_result.meshes)
			{
				//create_box(objdata.position, objdata.scale);
			}

			//GDENG03 GAME TIME HO
			std::vector<RigidObject*> cubes;

			for (int x = -5; x < 5; x++)
			{
				for (int y = -5; y < 5; y++)
				{
					auto depth = (x + y) % 3;

					auto ho_cube = create_box(Vector3(x, y, 5 + depth), Vector3(0.5f, 0.5f, 0.5f));
					cubes.push_back(ho_cube);
				}
			}

			float* ho_time = new float();

			auto animator = new GenericObject([cubes, ho_time, test_mat](GenericObject* self, float deltatime) {
				Vector3 from_rot = Vector3(0, 0, 0);
				Vector3 to_rot = Vector3(0, 180, 180);
				float animation_time = 0.5f;
				(*ho_time) += deltatime;
				auto anim_t = (*ho_time) * animation_time;

				int coeffienct_counter = 0;
				for (auto const& cube : cubes)
				{
					coeffienct_counter++;
					float coefficient = ((coeffienct_counter % 4) - 2) * 0.5f;
					if (coefficient == 0)
						coefficient = 1;

					cube->Local().rotation.Set(Quaternion::Euler(Vector3::Lerp(from_rot, to_rot, anim_t * coefficient)));
				}
			});
			animator->SetParent(game_root);

#pragma endregion
			return game_root;
			};

#pragma endregion
		auto initial_root = create_main_game();
		this->current_root = initial_root;
#pragma region MAIN LOOP
		
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