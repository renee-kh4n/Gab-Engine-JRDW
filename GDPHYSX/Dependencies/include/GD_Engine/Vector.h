#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <cmath>

constexpr auto VECTORSIZE = 3;

namespace gde {
	struct Vector3 {
	private:
		glm::vec3 asGlm() {
			return glm::vec3(this->x, this->y, this->z);
		}
	
	public:
		union {
			struct {
				float x, y, z;
			};
			float components[VECTORSIZE];
		};

		Vector3();
		Vector3(float x, float y, float z);

		float SqrMagnitude();

		float Magnitude();

		Vector3 Normalized();

		Vector3 ComponentMult(const Vector3& right);

		float Dot(const Vector3& right);

		Vector3 Cross(const Vector3& right);

		void operator=(const Vector3& right);
		void operator+=(const Vector3& right);
		void operator-=(const Vector3& right);

		void operator*=(const float& right)
		{
			for (int i = 0; i < VECTORSIZE; i++)
			{
				this->components[i] *= right;
			}
		}

		Vector3 operator-();
		Vector3 operator*(const float& right);
		Vector3 operator+(const Vector3& right);
		Vector3 operator-(const Vector3& right);

		operator glm::vec3() const {
			return glm::vec3(this->x, this->y, this->z);
		}

		std::string ToString();

		//constants
		static const Vector3 zero;
	};
}