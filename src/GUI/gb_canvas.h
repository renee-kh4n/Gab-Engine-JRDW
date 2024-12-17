#pragma once

#include "gb_rect.h"
#include <glm/matrix.hpp>

#include <glad/glad.h>

#include <list>

namespace gbe {
	namespace gui {
		class gb_canvas {
		private:
			glm::ivec2 reference_resolution;
			std::list<gb_rect*> children;
		public:
			void DrawToCurrentBuffer(glm::ivec2 target_resolution);
			glm::ivec2 Get_reference_resolution();
			std::list<gb_rect*>& Get_children();
		};
	}
}