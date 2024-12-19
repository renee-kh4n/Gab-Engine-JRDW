#pragma once

#include <cmath>
#include <glm/vec2.hpp>

namespace gbe {
	struct Vector2 : public glm::vec2 {
		Vector2();
		Vector2(float, float);
		Vector2(const glm::vec2& glmvec);

		Vector2& operator +=(const Vector2& b);
		Vector2& operator -=(const Vector2& b);

		const float* Get_Ptr();

		static const Vector2 zero;
	};
}