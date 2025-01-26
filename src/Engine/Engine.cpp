
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
		auto water_shader = new asset::Shader("DefaultAssets/Shaders/water.shader.gbe");
		auto depthoffield_shader = new asset::Shader("DefaultAssets/Shaders/depthoffield.shader.gbe");
		auto litShader = new asset::Shader("DefaultAssets/Shaders/lit.shader.gbe");
		auto bubbleShader = new asset::Shader("DefaultAssets/Shaders/bubble.shader.gbe");
		auto unlitShader = new asset::Shader("DefaultAssets/Shaders/unlit.shader.gbe");
		auto uiShader = new asset::Shader("DefaultAssets/Shaders/gui.shader.gbe");
		auto flipShader = new asset::Shader("DefaultAssets/Shaders/lit.shader_flip.gbe");
		auto flip2Shader = new asset::Shader("DefaultAssets/Shaders/lit.shader_flip2.gbe");

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
		
		auto door_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/door.png.gbe");
		auto duck_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/duck.png.gbe");
		auto hair_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/hair.png.gbe");
		auto hat_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/hat.png.gbe");
		auto pipes_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/pipes.png.gbe");
		auto sink_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/sink.png.gbe");
		auto slowdown_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/slowdown.png.gbe");
		auto slowdown_alpha_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/slowdown_alpha.png.gbe");
		auto soap_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/soap.png.gbe");
		auto tile_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/tile.png.gbe");
		auto tissue_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/tissue.png.gbe");
		auto toiletroll_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/toilet roll.png.gbe");
		auto toilet_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/toilet.png.gbe");
		auto toothbrush_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/toothbrush.png.gbe");
		auto toothpaste_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/toothpaste.png.gbe");
		auto tub_tex = new asset::Texture("DefaultAssets/Tex/Maps/Final/tub.png.gbe");

		//MESH CACHING
		auto quad_mesh = new Mesh("DefaultAssets/3D/plane.obj");
		auto sphere_mesh = new Mesh("DefaultAssets/3D/sphere.obj");
		auto cube_mesh = new Mesh("DefaultAssets/3D/cube.obj");
		auto level_mesh = new Mesh("DefaultAssets/3D/level_v7.obj");
		auto player_mesh = new Mesh("DefaultAssets/3D/Rubber Ducky.obj");
		auto hatplayer_mesh = new Mesh("DefaultAssets/3D/FinalLevel/playerhat.obj");

		//BY MATERIAL
		

		//MATERIAL CACHING
		auto unlit_white_mat = new Material(unlitShader);
		unlit_white_mat->setOverride("color", Vector4(1, 1, 1, 1));

		auto lit_water_mat = new Material(water_shader);
		lit_water_mat->setOverride("color", Vector4(0.6, 0.8, 1, 1));

		auto lit_white_mat = new Material(litShader);
		lit_white_mat->setOverride("color", Vector4(1, 1, 1, 1));
		lit_white_mat->setOverride("ambientLightTint", Vector3(1, 1, 1) * 0.4f);
		lit_white_mat->setOverride<bool>("hasDiffuseTex", true);
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

		//LEVELS

		

		Mesh* blue_mesh = new Mesh("DefaultAssets/3D/FinalLevel/blue.obj");
		Mesh* door_mesh = new Mesh("DefaultAssets/3D/FinalLevel/door.obj");
		Mesh* duck_mesh = new Mesh("DefaultAssets/3D/FinalLevel/duck.obj");
		Mesh* hairdryer_mesh = new Mesh("DefaultAssets/3D/FinalLevel/hair dryer.obj");
		Mesh* hat_mesh = new Mesh("DefaultAssets/3D/FinalLevel/hat.obj");
		Mesh* marble_mesh = new Mesh("DefaultAssets/3D/FinalLevel/marble.obj");
		Mesh* pipes_mesh = new Mesh("DefaultAssets/3D/FinalLevel/pipes.obj");
		Mesh* sink_mesh = new Mesh("DefaultAssets/3D/FinalLevel/sink.obj");
		Mesh* slide_mesh = new Mesh("DefaultAssets/3D/FinalLevel/slide.obj");
		Mesh* slowdown_mesh = new Mesh("DefaultAssets/3D/FinalLevel/slow down.obj");
		Mesh* soap_mesh = new Mesh("DefaultAssets/3D/FinalLevel/soap.obj");
		Mesh* speedboost_mesh = new Mesh("DefaultAssets/3D/FinalLevel/speed boost.obj");
		Mesh* tiles_mesh = new Mesh("DefaultAssets/3D/FinalLevel/tiles.obj");
		Mesh* tissue_mesh = new Mesh("DefaultAssets/3D/FinalLevel/tissue.obj");
		Mesh* toiletroll_mesh = new Mesh("DefaultAssets/3D/FinalLevel/toilet roll.obj");
		Mesh* toilet_mesh = new Mesh("DefaultAssets/3D/FinalLevel/toilet.obj");
		Mesh* toothbrush_mesh = new Mesh("DefaultAssets/3D/FinalLevel/toothbrush.obj");
		Mesh* toothpaste_mesh = new Mesh("DefaultAssets/3D/FinalLevel/toothpaste.obj");
		Mesh* tub_mesh = new Mesh("DefaultAssets/3D/FinalLevel/tub.obj");
		Mesh* water_mesh = new Mesh("DefaultAssets/3D/FinalLevel/water.obj");
		Mesh* whit_mesh = new Mesh("DefaultAssets/3D/FinalLevel/white.obj");

		Material* m_blue = new Material(flipShader);
		
		Material* m_door = new Material(flipShader);
		auto m_doortex = MaterialTexture();
		m_doortex.parameterName = "texdiffuse";
		m_doortex.textureRef.Assign(door_tex);
		m_door->textureOverrides.push_back(m_doortex);
		m_door->setOverride<bool>("hasDiffuseTex", true);

		Material* m_duck = new Material(flipShader);
		auto m_ducktex = MaterialTexture();
		m_ducktex.parameterName = "texdiffuse";
		m_ducktex.textureRef.Assign(duck_tex);
		m_duck->textureOverrides.push_back(m_ducktex);
		m_duck->setOverride<bool>("hasDiffuseTex", true);

		Material* m_hairdryer = new Material(flipShader);
		auto m_hairdryertex = MaterialTexture();
		m_hairdryertex.parameterName = "texdiffuse";
		m_hairdryertex.textureRef.Assign(hair_tex);
		m_hairdryer->textureOverrides.push_back(m_hairdryertex);
		m_hairdryer->setOverride<bool>("hasDiffuseTex", true);

		Material* m_hat = new Material(flipShader);
		auto m_hattex = MaterialTexture();
		m_hattex.parameterName = "texdiffuse";
		m_hattex.textureRef.Assign(hat_tex);
		m_hat->textureOverrides.push_back(m_hattex);
		m_hat->setOverride<bool>("hasDiffuseTex", true);

		Material* m_marble = new Material(flipShader);
		m_marble->setOverride<bool>("hasDiffuseTex", false);

		Material* m_pipes = new Material(flipShader);
		auto m_pipestex = MaterialTexture();
		m_pipestex.parameterName = "texdiffuse";
		m_pipestex.textureRef.Assign(pipes_tex);
		m_pipes->textureOverrides.push_back(m_pipestex);
		m_pipes->setOverride<bool>("hasDiffuseTex", true);

		Material* m_sink = new Material(flipShader);
		auto m_sinktex = MaterialTexture();
		m_sinktex.parameterName = "texdiffuse";
		m_sinktex.textureRef.Assign(sink_tex);
		m_sink->textureOverrides.push_back(m_sinktex);
		m_sink->setOverride<bool>("hasDiffuseTex", true);

		Material* m_slide = new Material(litShader);
		Material* m_slowdown = new Material(flipShader);
		Material* m_soap = new Material(litShader);
		auto m_soaptex = MaterialTexture();
		m_soaptex.parameterName = "texdiffuse";
		m_soaptex.textureRef.Assign(soap_tex);
		m_soap->textureOverrides.push_back(m_soaptex);
		m_soap->setOverride<bool>("hasDiffuseTex", true);

		Material* m_speedboost = new Material(flipShader);
		Material* m_tiles = new Material(flipShader);
		auto m_tilestex = MaterialTexture();
		m_tilestex.parameterName = "texdiffuse";
		m_tilestex.textureRef.Assign(tile_tex);
		m_tiles->textureOverrides.push_back(m_tilestex);
		m_tiles->setOverride<bool>("hasDiffuseTex", true);

		Material* m_tissue = new Material(flipShader);
		auto m_tissuetex = MaterialTexture();
		m_tissuetex.parameterName = "texdiffuse";
		m_tissuetex.textureRef.Assign(tissue_tex);
		m_tissue->textureOverrides.push_back(m_tissuetex);
		m_tissue->setOverride<bool>("hasDiffuseTex", true);

		Material* m_toiletroll = new Material(flipShader);
		auto m_toiletrolltex = MaterialTexture();
		m_toiletrolltex.parameterName = "texdiffuse";
		m_toiletrolltex.textureRef.Assign(toiletroll_tex);
		m_toiletroll->textureOverrides.push_back(m_toiletrolltex);
		m_toiletroll->setOverride<bool>("hasDiffuseTex", true);

		Material* m_toilet = new Material(flipShader);
		auto m_toilettex = MaterialTexture();
		m_toilettex.parameterName = "texdiffuse";
		m_toilettex.textureRef.Assign(toilet_tex);
		m_toilet->textureOverrides.push_back(m_toilettex);
		m_toilet->setOverride<bool>("hasDiffuseTex", true);

		Material* m_toothbrush = new Material(flipShader);
		auto m_toothbrushtex = MaterialTexture();
		m_toothbrushtex.parameterName = "texdiffuse";
		m_toothbrushtex.textureRef.Assign(toothbrush_tex);
		m_toothbrush->textureOverrides.push_back(m_toothbrushtex);
		m_toothbrush->setOverride<bool>("hasDiffuseTex", true);

		Material* m_toothpaste = new Material(flipShader);
		auto m_toothpastetex = MaterialTexture();
		m_toothpastetex.parameterName = "texdiffuse";
		m_toothpastetex.textureRef.Assign(toothpaste_tex);
		m_toothpaste->textureOverrides.push_back(m_toothpastetex);
		m_toothpaste->setOverride<bool>("hasDiffuseTex", true);

		Material* m_tub = new Material(flipShader);
		auto m_tubtex = MaterialTexture();
		m_tubtex.parameterName = "texdiffuse";
		m_tubtex.textureRef.Assign(tub_tex);
		m_tub->textureOverrides.push_back(m_tubtex);
		m_tub->setOverride<bool>("hasDiffuseTex", true);
		
		Material* m_water = new Material(flipShader);
		m_water->setOverride<bool>("hasDiffuseTex", false);

		Material* m_whit = new Material(flip2Shader);

		m_blue->setOverride("color", Vector4(0.4, 0.4, 1, 1));
		m_door->setOverride("color", Vector4(1, 1, 1, 1));
		m_duck->setOverride("color", Vector4(1,1,1,1));
		m_hairdryer->setOverride("color", Vector4(1,1,1,1));
		m_hat->setOverride("color", Vector4(1,1,1,1));
		m_marble->setOverride("color", Vector4(1,0.7,0.7,1));
		m_pipes->setOverride("color", Vector4(1,1,1,1));
		m_sink->setOverride("color", Vector4(1,1,1,1));
		m_slide->setOverride("color", Vector4(1,1,0,1));
		m_slowdown->setOverride("color", Vector4(1,1,1,1));
		m_soap->setOverride("color", Vector4(1,1,1,1));
		m_speedboost->setOverride("color", Vector4(1,1,1,1));
		m_tiles->setOverride("color", Vector4(1,1,1,1));
		m_tissue->setOverride("color", Vector4(1,1,1,1));
		m_toiletroll->setOverride("color", Vector4(1,1,1,1));
		m_toilet->setOverride("color", Vector4(1,1,1,1));
		m_toothbrush->setOverride("color", Vector4(1,1,1,1));
		m_toothpaste->setOverride("color", Vector4(1,1,1,1));
		m_tub->setOverride("color", Vector4(1,1,1,1));
		m_water->setOverride("color", Vector4(0.6,0.6,1,1));
		m_whit->setOverride("color", Vector4(1, 1, 1, 1));

		DrawCall* m_blue_dc = new DrawCall(blue_mesh, m_blue);
		DrawCall* m_door_dc = new DrawCall(door_mesh, m_door);
		DrawCall* m_duck_dc = new DrawCall(duck_mesh, m_duck);
		DrawCall* m_hairdryer_dc = new DrawCall(hairdryer_mesh, m_hairdryer);
		DrawCall* m_hat_dc = new DrawCall(hat_mesh, m_hat);
		DrawCall* m_marble_dc = new DrawCall(marble_mesh, m_marble);
		DrawCall* m_pipes_dc = new DrawCall(pipes_mesh, m_pipes);
		DrawCall* m_sink_dc = new DrawCall(sink_mesh, m_sink);
		DrawCall* m_slide_dc = new DrawCall(slide_mesh, m_slide);
		DrawCall* m_slowdown_dc = new DrawCall(slowdown_mesh, m_slowdown);
		DrawCall* m_soap_dc = new DrawCall(soap_mesh, m_soap);
		DrawCall* m_speedboost_dc = new DrawCall(speedboost_mesh, m_speedboost);
		DrawCall* m_tiles_dc = new DrawCall(tiles_mesh, m_tiles);
		DrawCall* m_tissue_dc = new DrawCall(tissue_mesh, m_tissue);
		DrawCall* m_toiletroll_dc = new DrawCall(toiletroll_mesh, m_toiletroll);
		DrawCall* m_toilet_dc = new DrawCall(toilet_mesh, m_toilet);
		DrawCall* m_toothbrush_dc = new DrawCall(toothbrush_mesh, m_toothbrush);
		DrawCall* m_toothpaste_dc = new DrawCall(toothpaste_mesh, m_toothpaste);
		DrawCall* m_tub_dc = new DrawCall(tub_mesh, m_tub);
		DrawCall* m_water_dc = new DrawCall(water_mesh, m_water);
		DrawCall* m_whit_dc = new DrawCall(whit_mesh, m_whit);

		std::vector<DrawCall*> level_drawcalls = {
			m_blue_dc,
			m_door_dc,
			m_duck_dc,
			m_hairdryer_dc,
			m_hat_dc,
			m_marble_dc,
			m_pipes_dc,
			m_sink_dc,
			m_slide_dc,
			m_slowdown_dc,
			m_soap_dc,
			m_speedboost_dc,
			m_tiles_dc,
			m_tissue_dc,
			m_toiletroll_dc,
			m_toilet_dc,
			m_toothbrush_dc,
			m_toothpaste_dc,
			m_tub_dc,
			m_water_dc,
			m_whit_dc
		};

		for (auto dc : level_drawcalls)
		{
			mRenderPipeline->RegisterDrawCall(dc);
		}

		//DRAWCALL CACHING
		auto line_drawcall = new DrawCall(quad_mesh, unlit_white_mat);
		mRenderPipeline->RegisterDrawCall(line_drawcall);

		auto whiteball_drawcall = new DrawCall(sphere_mesh, lit_white_mat);
		mRenderPipeline->RegisterDrawCall(whiteball_drawcall);

		auto water_quad = new DrawCall(quad_mesh, lit_water_mat);
		mRenderPipeline->RegisterDrawCall(water_quad);

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
		
		auto level_drawcall = new DrawCall(level_mesh, lit_white_mat);
		mRenderPipeline->RegisterDrawCall(level_drawcall);

		auto duck_drawcall = new DrawCall(player_mesh, m_duck);
		mRenderPipeline->RegisterDrawCall(duck_drawcall);
		auto duckhat_drawcall = new DrawCall(hatplayer_mesh, m_hat);
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
		mInputSystem->RegisterActionListener(player_name, new MouseDeltaImplementation());
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::SPACE>());
		mInputSystem->RegisterActionListener(player_name, new KeyPressImplementation<Keys::ESCAPE>());
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
				audio_ui_click->Play();
				});
			button->Set_onHoverAction([=]() {
				audio_ui_hover->Play();
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
			mWindow->Set_cursorLock(false);
			//GUI canvases
			gbe::gui::gb_canvas* mainmenu_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
			mGUIPipeline->SetActiveCanvas(mainmenu_canvas);

			gbe::gui::gb_image* main_image = new gbe::gui::gb_image();
			main_image->Set_Image(mainmenu_tex);
			main_image->bl_pivot = Vector2(-1, -1);
			main_image->tr_pivot = Vector2(1, 1);
			main_image->bl_offset = Vector2(0, 0);
			main_image->tr_offset = Vector2(0, 0);
			main_image->Set_handleType(gui::gb_rect::PointerEventHandleType::PASS);
			mainmenu_canvas->AddRootChild(main_image);

			create_image_button(startbubble_tex, mainmenu_canvas,
				Vector2(-0.53, -0.55),
				Vector2(120, 80),
				[=]() {
					this->ChangeRoot(create_main_game());
				});
			create_image_button(exitbubble_tex, mainmenu_canvas,
				Vector2(-0.13, -0.59),
				Vector2(120, 80),
				[=]() {
					mWindow->Terminate();
				});

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

			return game_root;
			};

		create_main_game = [&]() {
			auto game_root = this->CreateBlankRoot();
			mWindow->Set_cursorLock(true);
			//SCENE GUI
			gbe::gui::gb_canvas* maingame_canvas = new gbe::gui::gb_canvas(Vector2Int(800, 800));
			mGUIPipeline->SetActiveCanvas(maingame_canvas);
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
				hairspray_trigger->Local().scale.Set(Vector3(1, 3, 1));
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
						audio_hairdryer->Play();
					}
					});
				auto hairspray_particle_system = new ParticleSystem(create_particle);
				hairspray_particle_system->SetParent(hairspray);
				auto bl_corner_bound = Vector3(-1, -1, -1);
				hairspray_particle_system->SetBounds(bl_corner_bound, -bl_corner_bound);
				hairspray_particle_system->Set_force(Vector3(0, 800, 0));
				hairspray_particle_system->Set_rate(10);
				hairspray_particle_system->Set_lifetime(1);
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
			auto create_paste = [=](Vector3 pos) {

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
			//auto directional_light = new DirectionalLight();
			//directional_light->Set_Color(Vector3(1, 1, 1));
			//directional_light->Set_Intensity(1);
			//directional_light->Local().rotation.Set(Quaternion::Euler(Vector3(80, 90, 0)));
			//directional_light->SetParent(game_root);
			//directional_light->Set_ShadowmapResolutions(2160);

			//Player and Camera setup
			auto f_speed = 100.0f;
			auto f_jump = 180.0f;

			PerspectiveCamera* player_cam = new PerspectiveCamera(mWindow);

			player->SetParent(game_root);
			player->Local().scale.Set(Vector3(1, 1, 1) * 0.8f);

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
				auto ppos = player->World().position.Get();
				self->SetWorldPosition(ppos);
				});
			camera_parent->SetParent(game_root);
			player_cam->angles = 80;
			player_cam->nearClip = 1.0f;
			player_cam->farClip = 200.0f;
			player_cam->WorldUp = Vector3(0, 1, 0);
			player_cam->SetParent(camera_parent);
			auto target_campos = Vector3(0, 2, -5);
			player_cam->TranslateWorld(target_campos);

			//camera clip check
			auto cameraclipcheck = [=]() {
				Vector3 campos_world = Vector3();
				campos_world.y = 2;
				campos_world += camera_parent->World().GetForward() * -5.0f;

				auto check = physics::Raycast(camera_parent->World().position.Get(), campos_world);
				if (check.result) {
					return (Vector3)(check.intersection - (campos_world * 0.1f));
				}
				else {
					return campos_world;
				}
			};

			auto camera_check = new GenericObject([=](GenericObject* self, float delta) {
				auto campos = cameraclipcheck();
				player_cam->World().position.Set(campos);
				});
			camera_check->SetParent(camera_parent);

			//ground check
			auto groundcheck = [=]() {
				auto floorcheck = physics::Raycast(player->World().position.Get(), Vector3(0, -1.5f, 0));
				return floorcheck.result;
				};
			//BALL SHOOTER
			auto shoot_func = [=]() {
				auto spawnpos = camera_parent->World().position.Get() - (camera_parent->World().GetUp() * 0.3f);

				auto ball = spawnball(spawnpos, 0.2f);
				ball->GetRigidbody()->AddForce((physics::PhysicsVector3)(camera_parent->World().GetForward() * 1000.0f));
				audio_shoot->Play();
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
				shoot_func();

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

					audio_jump->Play();
				}

				}));
			//ESCAPE Customer
			input_communicator->AddCustomer(new InputCustomer<KeyPress<Keys::ESCAPE>>([=](KeyPress<Keys::ESCAPE>* value, bool changed) {
				if (value->state != KeyPress<Keys::ESCAPE>::START)
					return;

				ChangeRoot(create_main_menu());

				}));
			input_communicator->SetParent(player_input);


			//Death Trigger
			auto death_checker = new GenericObject([=](GenericObject* self, float delta) {
				if (player->World().position.Get().y < -4) {
					player->SetWorldPosition(Vector3::zero);
					player->GetRigidbody()->Set_velocity(Vector3::zero);

					audio_jump->Play();
				}
				});
			death_checker->SetParent(game_root);
#pragma endregion
			
			//Actual objects
			auto level_object = new RigidObject(true);
			level_object->TranslateWorld(Vector3(10, -6, -27));
			level_object->SetParent(game_root);
			
			
			for (auto dc : level_drawcalls)
			{
				auto level_dc_renderer = new RenderObject(dc);
				level_dc_renderer->SetParent(level_object);
			}

			auto level_collider = new MeshCollider(level_mesh);
			level_collider->SetParent(level_object);

			create_spray(Vector3(24, -3, -3));
			create_spray(Vector3(24, 0, -12));
			create_spray(Vector3(-1, 0, -30));
			create_spray(Vector3(8, 0, -46));
			create_spray(Vector3(11.6, 4.6, -51.7));
		

			//Goal
			auto goal_trigger = new GenericObject([=](GenericObject* self, float delta) {
				Vector3 player_delta = self->World().position.Get() - player->World().position.Get();
				auto sd_player = player_delta.SqrMagnitude();

				if (sd_player * sd_player < 7 * 7) {
					ChangeRoot(create_main_menu());
				}

				});
			goal_trigger->TranslateWorld(Vector3(25, -3, -38));
			goal_trigger->SetParent(game_root);

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