#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

#include "Vector_base.h"

namespace gde{
	struct Vector3 : internal::Vector<3, float> {
	public:
		union{
			struct {
				float x, y, z;
			};
			float values[3];
		};

		Vector3();
		Vector3(const Vector3& from);
		Vector3(float x, float y, float z);

		virtual float* GetValuePointer(int index);

		Vector3(glm::vec3 glmvec);
		operator glm::vec3() const;

		static const Vector3 zero;

		// Inherited via Vector_base
		Vector3 Cross(const Vector3& right);

		Vector3 operator-();
		Vector3 operator*(const float& right);
		Vector3 operator+(const Vector3& right);
		Vector3 operator-(const Vector3& right);
	};
}