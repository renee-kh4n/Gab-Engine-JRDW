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

		bool pointer_inUi;
		bool keyboard_inUi;

		bool mouse_holding;
		Vector3 mouse_startdrag_dir;
		Vector3 dragging_original_position;
		float drag_depth;


		//GIZMOS
		PhysicsObject* f_gizmo;
		PhysicsObject* r_gizmo;
		PhysicsObject* u_gizmo;
	public:
		Editor(RenderPipeline* renderpipeline, Window* window, Engine* engine);
		void PrepareFrame();
		void DrawFrame();
		void ProcessRawWindowEvent(void* rawwindowevent);
		void PresentFrame();
		void RenderPass(VkCommandBuffer cmd);
	};
}