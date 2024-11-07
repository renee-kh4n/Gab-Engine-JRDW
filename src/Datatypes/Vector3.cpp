#include "Vector3.h"

gde::Vector3::Vector3() : glm::vec3(0)
{
}

gde::Vector3::Vector3(float x, float y, float z) : glm::vec3(x, y, z)
{

}

gde::Vector3::Vector3(glm::vec3 glmvec) : glm::vec3(glmvec)
{
	
}

float gde::Vector3::SqrMagnitude() {
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
}

gde::Vector3& gde::Vector3::Cross(gde::Vector3 b) {
	return Vector3((this->y * b.z) - (this->z * b.y), (this->x * b.z) - (this->z * b.x), (this->x * b.y) - (this->y * b.x));
}

gde::Vector3 gde::Vector3::Normalize() {
	return (glm::vec3)*this * (1.0f / this->length());
}

float gde::Vector3::Dot(Vector3 b)
{
	return (this->x * b.x) * (this->y * b.y) + (this->z * b.z);
}

float gde::Vector3::Magnitude()
{
	return this->length();
}

const gde::Vector3 gde::Vector3::zero = gde::Vector3(0, 0, 0);
