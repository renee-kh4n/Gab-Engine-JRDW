#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <list>

namespace gbe {
	namespace gui {
		class gb_canvas;

		class gb_rect {
		private:
			gb_canvas* belongs_to;

			glm::vec2 bl_offset;
			glm::vec2 tr_offset;

			glm::vec2 bl_anchor;
			glm::vec2 tr_anchor;

			std::list<gb_rect*> children;
		public:
			virtual void Render(glm::mat4& parent_transform);
			virtual void Get_BlocksPointerEvents() = 0;
		};
	}
}