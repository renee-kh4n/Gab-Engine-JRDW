#pragma once

#include <glm/vec2.hpp>

#include "gb_canvas.h"
#include "interfaces/gb_onClick.h"
#include "interfaces/gb_onEnter.h"
#include "interfaces/gb_onExit.h"
#include "Asset/AssetTypes/Shader.h"

namespace gbe {
	namespace gui {
		class gbuiPipeline {
		private:
			static gbuiPipeline* instance;

			glm::vec2 mousepos_screen_centered;
			glm::ivec2 target_resolution;
			gb_canvas* active_canvas;
			gb_rect* current_object_on_cursor;

			unsigned int quad_vao;
			unsigned int main_framebuffer;
			asset::Shader* gui_shader;
		public:
			gbuiPipeline(unsigned int quad_vao, unsigned int main_framebuffer, asset::Shader* gui_shader);
			static gbuiPipeline* Get_current();

			unsigned int Get_main_framebuffer();
			unsigned int Get_quad_vao();
			asset::Shader* Get_gui_shader();

			void SetActiveCanvas(gb_canvas* canvas);
			void DrawActiveCanvas();
			void DrawElement(gbe::gui::gb_rect* element);
			void Update(double delta);

			void Set_target_resolution(glm::ivec2 target_resolution);
			void PassScreenSpaceMousePos(glm::ivec2 mousepos);
			void Click();
		};
	}
}