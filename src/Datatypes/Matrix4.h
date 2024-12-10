#pragma once

#include <glm/mat4x4.hpp>

namespace gbe {
	struct Matrix4 : public glm::mat4x4 {
		Matrix4();
		Matrix4(const glm::mat4x4& from);

		const float* Get_Ptr();
	};
}