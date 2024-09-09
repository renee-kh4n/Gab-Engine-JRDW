#pragma once

#include <string>

#include <vec4.hpp>
#include <cmath>
#include <vector>

#include "Vector_base.h"

namespace gde {
	struct Vector4 : internal::Vector<4, float> {
	public:
		union {
			struct {
				float x, y, z, w;
			};
			float values[4];
		};

		Vector4();
		Vector4(const Vector4& from);
		Vector4(float x, float y, float z, float w);

		Vector4(glm::vec4 glmvec);
		operator glm::vec4() const;
	};
}