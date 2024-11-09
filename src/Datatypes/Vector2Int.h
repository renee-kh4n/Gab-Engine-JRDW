#pragma once

#include <cmath>
#include <vector>

#include <glm/vec2.hpp>

namespace gbe {
	struct Vector2Int : public glm::ivec2 {
		Vector2Int();
		Vector2Int(int, int);
		Vector2Int(glm::ivec2 glmvec);

		static const Vector2Int zero;
	};
}