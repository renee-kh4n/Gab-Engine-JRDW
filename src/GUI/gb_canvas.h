#pragma once

#include "gb_rect.h"
#include "rect_types/gb_root.h"
#include <glm/matrix.hpp>

#include <glad/glad.h>

#include <list>

namespace gbe {
	namespace gui {
		class gb_canvas {
		private:
			glm::ivec2 reference_resolution;
			gb_root root;
		public:
			gb_canvas(glm::ivec2 reference_resolution);

			gb_rect* QueryAtPosition(glm::vec2 normalized_centered_position);
			void Draw(glm::ivec2 target_resolution);
			glm::ivec2 Get_reference_resolution();
			void Update(double delta);

			void AddRootChild(gb_rect* other);
		};
	}
}