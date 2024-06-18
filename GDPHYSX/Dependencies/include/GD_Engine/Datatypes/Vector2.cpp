#include "Vector2.h"

namespace gde {
	Vector2::Vector2() : Vector<2, float>({ &x, &y })
	{
		this->x = 0;
		this->y = 0;
	}

	Vector2::Vector2(const Vector2& from) : Vector<2, float>({ &x, &y })
	{
		this->x = from.x;
		this->y = from.y;
	}

	Vector2::Vector2(float x, float y) : Vector<2, float>({ &x, &y})
	{
		this->x = x;
		this->y = y;
	}

	float* Vector2::GetValuePointer(int index)
	{
		return &values[index];
	}

	const Vector2 Vector2::zero = Vector2(0, 0);

	Vector2 Vector2::operator-()
	{
		Vector2 result = Vector2(*this);
		result *= -1;
		return result;
	}

	Vector2 Vector2::operator*(const float& right)
	{
		Vector2 result = Vector2(*this);
		result *= right;
		return result;
	}

	Vector2 Vector2::operator+(const Vector2& right)
	{
		Vector2 result = Vector2(*this);
		result += right;
		return result;
	}
	Vector2 Vector2::operator-(const Vector2& right)
	{
		Vector2 result = Vector2(*this);
		result -= right;
		return result;
	}
}