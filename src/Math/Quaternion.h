#pragma once

#include <string>
#include <cmath>
#include <vector>

#include <glm/gtx/quaternion.hpp>
#include "Vector3.h"

namespace gbe {
	struct Quaternion : public glm::quat {
		Quaternion();
		Quaternion(glm::quat glmquat);

		static Quaternion AngleAxis(Vector3, float deg_angle);
		static Quaternion LookAtRotation(Vector3 forward, Vector3 Up);
		static Quaternion Euler(Vector3);

		operator glm::quat() const;

		Vector3 ToEuler();
	};
}