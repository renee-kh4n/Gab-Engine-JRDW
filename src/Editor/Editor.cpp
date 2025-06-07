#include "Editor.h"

#include "Graphics/gbe_graphics.h"
#include "Engine/gbe_engine.h"

gbe::Editor::Editor(RenderPipeline* renderpipeline, Window* window, Engine* engine, Time* _mtime)
{
	this->mengine = engine;
	this->mwindow = window;
	this->mrenderpipeline = renderpipeline;
	this->mtime = _mtime;

	//GET ALL REQUIRED VARIABLES FROM RENDERPIPELINE
	auto vkInst = static_cast<VkInstance*>(renderpipeline->GetPipelineVariable("VkInstance"));
	auto vkdevice = static_cast<VkDevice*>(renderpipeline->GetPipelineVariable("VkDevice"));
	auto vkphysicalDevice = static_cast<VkPhysicalDevice*>(renderpipeline->GetPipelineVariable("VkPhysicalDevice"));
	auto renderPass = static_cast<VkRenderPass*>(renderpipeline->GetPipelineVariable("VkRenderPass"));
	auto graphicsqueue = static_cast<VkQueue*>(renderpipeline->GetPipelineVariable("VkQueue_graphics"));


	//1: create descriptor pool for IMGUI
	// the size of the pool is very oversize, but it's copied from imgui demo itself.
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	if (vkCreateDescriptorPool(*vkdevice, &pool_info, nullptr, &imguiPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to init imgui");
	}


	// 2: initialize imgui library

	//this initializes the core structures of imgui
	ImGui::CreateContext();

	//this initializes imgui for SDL
	ImGui_ImplSDL2_InitForVulkan(static_cast<SDL_Window*>(window->Get_implemented_window()));

	//this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = *vkInst;
	init_info.PhysicalDevice = *vkphysicalDevice;
	init_info.Device = *vkdevice;
	init_info.Queue = *graphicsqueue;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.RenderPass = *renderPass;

	ImGui_ImplVulkan_Init(&init_info);

	/*
	//add the destroy the imgui created structures
	_mainDeletionQueue.push_function([=]() {
		vkDestroyDescriptorPool(_device, imguiPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		});
		*/

	//CREATE THE GIZMO OBJECTS
	auto gizmoshader = new asset::Shader("DefaultAssets/Shaders/gizmo.shader.gbe");
	this->gizmo_arrow_mesh = new asset::Mesh("DefaultAssets/3D/arrow.obj.gbe");

	auto mat_r = new asset::Material("DefaultAssets/Materials/gizmo.mat.gbe");
	auto mat_g = new asset::Material("DefaultAssets/Materials/gizmo.mat.gbe");
	auto mat_b = new asset::Material("DefaultAssets/Materials/gizmo.mat.gbe");
	mat_r->setOverride("color", Vector4(1, 0, 0, 1.0f));
	mat_g->setOverride("color", Vector4(0, 1, 0, 1.0f));
	mat_b->setOverride("color", Vector4(0, 0, 1, 1.0f));
	this->gizmo_arrow_drawcall_r = this->mrenderpipeline->RegisterDrawCall(this->gizmo_arrow_mesh, mat_r);
	this->gizmo_arrow_drawcall_g = this->mrenderpipeline->RegisterDrawCall(this->gizmo_arrow_mesh, mat_g);
	this->gizmo_arrow_drawcall_b = this->mrenderpipeline->RegisterDrawCall(this->gizmo_arrow_mesh, mat_b);
}

void gbe::Editor::CreateGizmoArrow(gbe::PhysicsObject*& out_g, DrawCall* drawcall, Vector3 rotation, Vector3 direction) {
	if (out_g == nullptr) {
		auto newGizmo = new RigidObject(true);
		newGizmo->SetParent(mengine->GetCurrentRoot());
		newGizmo->Local().scale.Set(Vector3(0.1f, 0.1f, (gizmo_offset_distance * 2.0f)));
		BoxCollider* FGizmo_collider = new BoxCollider();
		FGizmo_collider->SetParent(newGizmo);
		FGizmo_collider->Local().position.Set(Vector3(0, 0, 0));
		RenderObject* platform_renderer = new RenderObject(drawcall);
		platform_renderer->SetParent(newGizmo);

		out_g = newGizmo;
	}

	//SELECTED SPECIFIC THINGS
	auto rot = Quaternion::Euler(rotation);
	out_g->Local().position.Set(this->selected[0]->World().position.Get());
	out_g->Local().rotation.Set(this->selected[0]->World().rotation.Get() * rot);
	out_g->TranslateWorld(direction * gizmo_offset_distance);
}

void gbe::Editor::ProcessRawWindowEvent(void* rawwindowevent) {
	auto sdlevent = static_cast<SDL_Event*>(rawwindowevent);

	ImGui_ImplSDL2_ProcessEvent(sdlevent);

	//CHECK SHIFT CLICK
	bool shift_click = false;

	//CLICKED
	if (sdlevent->type == SDL_MOUSEBUTTONDOWN) {
		if (sdlevent->button.button == SDL_BUTTON_LEFT && !this->pointer_inUi) {

			//RAYCAST MECHANICS
			auto current_camera = this->mengine->GetCurrentRoot()->GetHandler<Camera>()->object_list.front();
			Vector3 camera_pos = current_camera->World().position.Get();
			auto mousedir = current_camera->ScreenToRay(mwindow->GetMouseDecimalPos());

			//OBJECT SELECTION
			Vector3 ray_dir = mousedir * 10000.0f;
			auto result = physics::Raycast(camera_pos, ray_dir);
			if (result.result) {
				//CHECK IF OTHER IS A GIZMO
				for (auto& gizmoptr : gizmos)
				{
					if (result.other == (*gizmoptr)) {
						held_gizmo = *gizmoptr;
					}
				}

				if (held_gizmo != nullptr) {
					this->original_selected_position = selected[0]->World().position.Get();
					std::cout << "Holding Gizmo" << std::endl;
				}
				else {
					if (!shift_click) { //CLEAR SELECTION IF NOT MULTISELECTING AND CLICKED SOMETHING ELSE
						this->selected.clear();
					}

					this->selected.push_back(result.other);
					this->selected_f = result.other->World().GetForward();
					this->selected_r = result.other->World().GetRight();
					this->selected_u = result.other->World().GetUp();

					//SPAWN GIZMO
					this->CreateGizmoArrow(this->f_gizmo, this->gizmo_arrow_drawcall_b, Vector3(0, 180, 0), this->selected_f);
					this->CreateGizmoArrow(this->r_gizmo, this->gizmo_arrow_drawcall_r, Vector3(0, -90, 0), this->selected_r);
					this->CreateGizmoArrow(this->u_gizmo, this->gizmo_arrow_drawcall_g, Vector3(90, 0, 0), this->selected_u);

				}
			}
			else { //NOTHING WAS CLICKED
				if (!shift_click) { //NOT MULTISELECTING
					//CLEAR SELECTION IF NOT MULTISELECTING AND CLICKED NOTHING
					this->selected.clear();

					//DELETE GIZMO
					for (auto& gizmoptr : this->gizmos)
					{
						if (*gizmoptr != nullptr) {
							(*gizmoptr)->Destroy();
							(*gizmoptr) = nullptr;
						}
					}
				}
			}
		}
	}
	if (sdlevent->type == SDL_MOUSEBUTTONUP) {
		if (sdlevent->button.button == SDL_BUTTON_LEFT) {
			this->held_gizmo = nullptr;
		}
	}
}

void gbe::Editor::PrepareFrame()
{
	//imgui new frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();

	auto& ui_io = ImGui::GetIO();

	this->pointer_inUi = ui_io.WantCaptureMouse;
	this->keyboard_inUi = ui_io.WantCaptureKeyboard;
}

void gbe::Editor::DrawFrame()
{
	//==============================EDITOR UPDATE==============================//
	if (held_gizmo != nullptr) {
		//FIND POSITION TO TRANSFORM THE SELECTED OBJECT
		//FIND DIRECTION
		Vector3 transformDirection;
		if (held_gizmo == f_gizmo)
			transformDirection = this->selected_f;
		if (held_gizmo == r_gizmo)
			transformDirection = this->selected_r;
		if (held_gizmo == u_gizmo)
			transformDirection = this->selected_u;

		auto current_camera = this->mengine->GetCurrentRoot()->GetHandler<Camera>()->object_list.front();
		Vector3 camera_pos = current_camera->World().position.Get();
		auto mousedir = current_camera->ScreenToRay(mwindow->GetMouseDecimalPos());

		// Find the closest point on the line segments
		Vector3 closestPointOnTransformDir = Vector3::GetClosestPointOnLineGivenLine(original_selected_position, transformDirection, camera_pos, mousedir);

		selected[0]->SetWorldPosition(closestPointOnTransformDir);

		//UPDATE THE POSITION OF ALL GIZMOS
		this->f_gizmo->Local().position.Set(closestPointOnTransformDir + (gizmo_offset_distance * this->selected_f));
		this->r_gizmo->Local().position.Set(closestPointOnTransformDir + (gizmo_offset_distance * this->selected_r));
		this->u_gizmo->Local().position.Set(closestPointOnTransformDir + (gizmo_offset_distance * this->selected_u));
	}

	//==============================IMGUI==============================//
	//IMGUI OBJECT INSPECTOR
	if (!ImGui::Begin("Inspector")) {
		ImGui::End();

		return;
	}

	const float label_width_base = ImGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
	const float label_width_max = ImGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
	const float label_width = label_width_base < label_width_max ? label_width_base : label_width_max;
	ImGui::PushItemWidth(-label_width);                                     // Right-align: framed items will leave 'label_width' available for the label.

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::InputDouble("GAME TIME: ", &mtime->scale);

	//TRANSFORM INSPECTOR
	if (this->selected.size() > 0)
	{
		//FOR ONLY ONE

		Vector3 refpos = this->selected[0]->Local().position.Get();
		Vector3 refrot = this->selected[0]->Local().rotation.Get().ToEuler();
		Vector3 refscale = this->selected[0]->Local().position.Get();

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::SeparatorText("Position");

			ImGui::InputFloat("X", &refpos.x);
			ImGui::InputFloat("Y", &refpos.y);
			ImGui::InputFloat("Z", &refpos.z);

			ImGui::SeparatorText("Rotation");

			ImGui::InputFloat("X", &refrot.x);
			ImGui::InputFloat("Y", &refrot.y);
			ImGui::InputFloat("Z", &refrot.z);

			ImGui::SeparatorText("Scale");

			ImGui::InputFloat("X", &refscale.x);
			ImGui::InputFloat("Y", &refscale.y);
			ImGui::InputFloat("Z", &refscale.z);
		}
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

void gbe::Editor::PresentFrame()
{
	ImGui::Render();
}

void gbe::Editor::RenderPass(VkCommandBuffer cmd)
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}
