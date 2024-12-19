#pragma once

#include <string>
#include <cmath>
#include <vector>

#include <glm/vec3.hpp>

namespace gbe{
	struct Vector3 : public glm::vec3 {
		Vector3();
		Vector3(float, float, float);
		Vector3(float);
		Vector3(const glm::vec3& glmvec);

		float SqrMagnitude();
		Vector3& Cross(Vector3);
		Vector3 Normalize();
		float Dot(Vector3 b);
		float Magnitude();

		Vector3& operator+=(Vector3 const& v);
		Vector3& operator-=(Vector3 const& v);
		operator glm::vec3() const;

		const float* Get_Ptr();

		static const Vector3 zero;
	};
}