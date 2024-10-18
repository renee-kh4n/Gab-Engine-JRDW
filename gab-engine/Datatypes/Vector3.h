#pragma once

#include <string>
#include <cmath>
#include <vector>

#include <glm/vec3.hpp>

namespace gde{
	struct Vector3 : glm::vec3 {
		Vector3();
		Vector3(float, float, float);
		Vector3(glm::vec3 glmvec);

		static const Vector3 zero;
	};
}