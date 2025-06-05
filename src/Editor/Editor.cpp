#include "Editor.h"

#include "Graphics/gbe_graphics.h"
#include "Engine/gbe_engine.h"

gbe::Editor::Editor(RenderPipeline* renderpipeline, Window* window, Engine* engine)
{
	this->mengine = engine;
	this->mwindow = window;
	this->mrenderpipeline = renderpipeline;

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
			auto enginecurroot = this->mengine->GetCurrentRoot();
			auto current_camera = this->mengine->GetCurrentRoot()->GetHandler<Camera>()->object_list.front();
			Vector3 camera_pos = current_camera->World().position.Get();
			auto mousedir = current_camera->ScreenToRay(mwindow->GetMouseDecimalPos());
			
			//OBJECT SELECTION
			Vector3 ray_dir = mousedir * 10000.0f;
			auto result = physics::Raycast(camera_pos, ray_dir);
			if (!shift_click) {
				this->selected.clear();
			}
			if (result.result) {
				this->selected.push_back(result.other);

				//SPAWN GIZMO

			}
		}
	}
	if (sdlevent->type == SDL_MOUSEBUTTONUP) {
		if (sdlevent->button.button == SDL_BUTTON_LEFT) {
			this->mouse_holding = false;
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
