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
		public:
			enum PointerEventHandleType {
				PASS,
				BLOCK,
				PARENT,
				BLOCK_AND_PARENT
			};
		protected:
			gb_canvas* canvas;

			PointerEventHandleType pointerEventHandleType = PointerEventHandleType::PASS;
			gb_rect* parent;
			std::list<gb_rect*> children;

			glm::mat4 world_transform;
		public:
			glm::vec2 bl_offset;
			glm::vec2 tr_offset;

			glm::vec2 bl_pivot;
			glm::vec2 tr_pivot;

			gb_rect();

			glm::mat4& Get_world_transform();

			PointerEventHandleType Get_handleType();
			void Set_handleType(PointerEventHandleType);
			gb_rect* QueryAtPosition(glm::vec2 normalized_centered_position);

			void UpdateTransform();
			virtual void Render();
			virtual void Update(float delta);

			void SetParent(gb_rect* other);
		};
	}
}