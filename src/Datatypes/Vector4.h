#pragma once

#include <string>
#include <cmath>
#include <vector>

#include <glm/vec4.hpp>

namespace gbe {
	struct Vector4 : public glm::vec4 {
		Vector4();
		Vector4(float, float, float, float);
		Vector4(glm::vec4 glmvec);

		static const Vector4 zero;
	};
}