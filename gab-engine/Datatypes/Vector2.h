#pragma once

#include <cmath>
#include <glm/vec2.hpp>

namespace gde {
	struct Vector2 : glm::vec2 {
		Vector2();
		Vector2(float, float);
		Vector2(glm::vec2 glmvec);

		static const Vector2 zero;
	};
}