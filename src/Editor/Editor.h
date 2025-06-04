#pragma once

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl2.h>

#include "Engine/gbe_engine.h"

namespace gbe {
	class RenderPipeline;
	class Engine;
	class Window;


	class Editor {
	private:
		Window* mwindow;
		Engine* mengine;
		RenderPipeline* mrenderpipeline;

		std::vector<gbe::Object*> selected;
	public:
		Editor(RenderPipeline* renderpipeline, Window* window, Engine* engine);
		void PrepareFrame();
		void DrawFrame();
		void ProcessRawWindowEvent(void* rawwindowevent);
		void PresentFrame();
		void RenderPass(VkCommandBuffer cmd);
	};
}