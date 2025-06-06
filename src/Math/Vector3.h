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

		float SqrMagnitude() const;
		Vector3 Cross(Vector3) const;
		Vector3 Normalize() const;
		float Dot(Vector3 b) const;
		float Magnitude() const;

		Vector3& operator+=(Vector3 const& v);
		Vector3& operator-=(Vector3 const& v);
		operator glm::vec3() const;

		const float* Get_Ptr();

		static const Vector3 zero;
		inline static Vector3 Lerp(Vector3& a, Vector3& b, float t) {
			auto d = b - a;
			d *= t;
			auto tv = a + d;
			return tv;
		}
		static const Vector3 RandomWithin(Vector3& a, Vector3& b);
		static Vector3 GetClosestPointOnLineGivenLine(const Vector3& a, const Vector3& adir, const Vector3& b, const Vector3& bdir);
	};
}