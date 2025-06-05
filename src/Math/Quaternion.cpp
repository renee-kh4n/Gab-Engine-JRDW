#include "Quaternion.h"

gbe::Quaternion::Quaternion() : glm::quat()
{
}

gbe::Quaternion::Quaternion(glm::quat glmquat) : glm::quat(glmquat)
{
}

gbe::Quaternion gbe::Quaternion::AngleAxis(Vector3 axis, float deg_angle)
{
	return glm::rotate(Quaternion(), glm::radians(deg_angle), (glm::vec3)axis);
}

gbe::Quaternion gbe::Quaternion::LookAtRotation(Vector3 forward, Vector3 Up)
{
	if (abs(forward.SqrMagnitude() - 1.0f) > 0.001f || abs(Up.SqrMagnitude() - 1.0f) > 0.001f) {
		throw "invalid parameters for look at rotation!";
	}

	auto lookatmat = glm::inverse(glm::lookAt((glm::vec3)Vector3::zero, (glm::vec3)forward, (glm::vec3)Up));

	return glm::quat_cast(lookatmat);
}

gbe::Quaternion gbe::Quaternion::Euler(Vector3 euler)
{
	return glm::quat(Vector3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z)));;
}

gbe::Vector3 gbe::Quaternion::ToEuler() {
	return Vector3(glm::eulerAngles(*this));
}

gbe::Quaternion::operator glm::quat() const
{
	return (glm::quat)*this;
}