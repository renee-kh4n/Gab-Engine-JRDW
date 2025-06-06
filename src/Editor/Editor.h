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
		Time* mtime;

		std::vector<gbe::Object*> selected;

		bool pointer_inUi;
		bool keyboard_inUi;

		//GIZMOS
		Vector3 original_selected_position;
		Vector3 selected_f;
		Vector3 selected_r;
		Vector3 selected_u;

		PhysicsObject* f_gizmo = nullptr;
		PhysicsObject* r_gizmo = nullptr;
		PhysicsObject* u_gizmo = nullptr;

		PhysicsObject* held_gizmo = nullptr;

		std::array<PhysicsObject**, 3> gizmos = {
			&f_gizmo,
			&r_gizmo,
			&u_gizmo
		};
	public:
		Editor(RenderPipeline* renderpipeline, Window* window, Engine* engine, Time* _mtime);
		void PrepareFrame();
		void DrawFrame();
		void ProcessRawWindowEvent(void* rawwindowevent);
		void PresentFrame();
		void RenderPass(VkCommandBuffer cmd);
	};
}