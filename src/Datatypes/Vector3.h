#pragma once

#include <string>
#include <cmath>
#include <vector>

#include <glm/vec3.hpp>

namespace gde{
	struct Vector3 : public glm::vec3 {
		Vector3();
		Vector3(float, float, float);
		Vector3(glm::vec3 glmvec);

		float SqrMagnitude();
		Vector3& Cross(Vector3);
		Vector3 Normalize();
		float Dot(Vector3 b);
		float Magnitude();

		Vector3& operator +=(const Vector3& b);
		Vector3& operator -=(const Vector3& b);

		static const Vector3 zero;
	};
}