#pragma once

#include <glm/glm.hpp>

namespace gbe {
	namespace rendering {
		struct Light {
			virtual enum Type {
				POINT,
				CONE,
				DIRECTION
			} GetType() = 0;

			glm::vec3 color;
			float intensity;
			glm::vec3 pos;

			bool changed = true;
			size_t previous_render_index = 0;
		};

		struct PointLight : public Light {
			// Inherited via Light
			virtual Type GetType() override;
		};

		struct ConeLight : public Light {
			// Inherited via Light
			virtual Type GetType() override;

			glm::vec3 dir;
			glm::vec2 angle;
		};

		struct DirLight : public Light {
			// Inherited via Light
			virtual Type GetType() override;

			glm::vec3 dir;
		};
	}
}