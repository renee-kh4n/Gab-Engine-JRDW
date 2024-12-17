#pragma once

#include <glm/vec2.hpp>

#include "gb_canvas.h"

namespace gbe {
	namespace gui {
		class gbuiPipeline {
		private:
			glm::vec2 mousepos_screen;
			gb_canvas* active_canvas;
			gb_rect* current_object_on_cursor;

			unsigned int quad_vao;
			unsigned int gui_shader_id;
		public:
			gbuiPipeline(unsigned int quad_vao, unsigned int gui_shader_id);

			void SetActiveCanvas(gb_canvas* canvas);
			void Draw(glm::ivec2 target_resolution);

			void PassScreenSpaceMousePos(glm::vec2 mousepos);
			void Click();
		};
	}
}