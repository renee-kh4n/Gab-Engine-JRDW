#include "Vector2Int.h"

namespace gde {
	Vector2Int::Vector2Int() : Vector<2, int>({ &x, &y })
	{
		this->x = 0;
		this->y = 0;
	}

	Vector2Int::Vector2Int(const gde::Vector2Int& from) : Vector<2, int>({ &x, &y })
	{
		this->x = from.x;
		this->y = from.y;
	}

	Vector2Int::Vector2Int(int x, int y) : Vector<2, int>({ &x, &y })
	{
		this->x = x;
		this->y = y;
	}

	int* Vector2Int::GetValuePointer(int index)
	{
		return &values[index];
	}

	Vector2Int Vector2Int::operator-()
	{
		Vector2Int result = Vector2Int(*this);
		result *= -1;
		return result;
	}

	Vector2Int Vector2Int::operator*(const float& right)
	{
		Vector2Int result = Vector2Int(*this);
		result *= right;
		return result;
	}

	Vector2Int Vector2Int::operator+(const Vector2Int& right)
	{
		Vector2Int result = Vector2Int(*this);
		result += right;
		return result;
	}
	Vector2Int Vector2Int::operator-(const Vector2Int& right)
	{
		Vector2Int result = Vector2Int(*this);
		result -= right;
		return result;
	}
}
