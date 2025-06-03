#pragma once

#include "Window/gbe_window.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_sdl2.h>

namespace gbe {
	class RenderPipeline;

	class Editor {
	private:
		Window* mwindow;
	public:
		Editor(RenderPipeline* renderpipeline, Window* window);
		void PrepareFrame();
		void DrawFrame();
		void PresentFrame();
		void RenderPass(VkCommandBuffer cmd);
	};
}