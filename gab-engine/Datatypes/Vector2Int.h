#pragma once

#include <cmath>
#include <vector>

#include "Vector_base.h"

namespace gde {
	struct Vector2Int : internal::Vector<2, int> {
	public:
		union {
			struct {
				int x, y;
			};
			int values[2];
		};

		Vector2Int();
		Vector2Int(const gde::Vector2Int& from);
		Vector2Int(int x, int y);

		virtual int* GetValuePointer(int index);

		Vector2Int operator-();
		Vector2Int operator*(const float& right);
		Vector2Int operator+(const Vector2Int& right);
		Vector2Int operator-(const Vector2Int& right);
	};
}