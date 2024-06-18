#pragma once

#include <cmath>
#include <vector>

#include "Vector_base.h"

namespace gde {
	struct Vector2 : internal::Vector<2, float> {
	public:
		union {
			struct {
				float x, y;
			};
			float values[2];
		};

		Vector2();
		Vector2(const Vector2& from);
		Vector2(float x, float y);

		virtual float* GetValuePointer(int index);

		static const Vector2 zero;

		Vector2 operator-();
		Vector2 operator*(const float& right);
		Vector2 operator+(const Vector2& right);
		Vector2 operator-(const Vector2& right);
	};
}