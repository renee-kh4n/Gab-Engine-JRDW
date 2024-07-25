#include "Vector3.h"

namespace gde {
	Vector3::Vector3() : Vector<3, float>({ &x, &y, &z })
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector3::Vector3(const Vector3& from) : Vector<3, float>({ &x, &y, &z })
	{
		this->x = from.x;
		this->y = from.y;
		this->z = from.z;
	}

	gde::Vector3::Vector3(float _x, float _y, float _z) : Vector<3, float>({ &x, &y, &z })
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	Vector3 Vector3::Cross(const Vector3& right)
	{
		Vector3 result = Vector3(0, 0, 0);

		result.x = (this->y * right.z) - (this->z * right.y);
		result.y = (this->z * right.x) - (this->x * right.z);
		result.z = (this->x * right.y) - (this->y * right.x);

		return result;
	}

	Vector3 Vector3::Normalize()
	{
		return (*this) * (1.0f / this->Magnitude());
	}

	Vector3::Vector3(glm::vec3 glmvec) : Vector<3, float>({&x, &y, &z})
	{
		this->x = glmvec.x;
		this->y = glmvec.y;
		this->z = glmvec.z;
	}
	Vector3::operator glm::vec3() const {
		return glm::vec3(this->x, this->y, this->z);
	}

	const Vector3 Vector3::zero = Vector3(0, 0, 0);

	Vector3 Vector3::operator-()
	{
		Vector3 result = Vector3(*this);
		result *= -1;
		return result;
	}

	Vector3 Vector3::operator*(const float& right)
	{
		Vector3 result = Vector3(*this);
		result *= right;
		return result;
	}

	Vector3 Vector3::operator+(const Vector3& right)
	{
		Vector3 result = Vector3(*this);
		result += right;
		return result;
	}
	Vector3 Vector3::operator-(const Vector3& right)
	{
		Vector3 result = Vector3(*this);
		result -= right;
		return result;
	}
}