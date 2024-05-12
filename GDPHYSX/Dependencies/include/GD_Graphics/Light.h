#pragma once

#include "util.h"

namespace gde {
	struct PointLight {
		glm::vec3 color;
		float intensity;
		glm::vec3 pos;
	};

	struct ConeLight {
		glm::vec3 color;
		float intensity;
		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec2 angle;
	};

	struct DirLight {
		glm::vec3 color;
		float intensity;
		glm::vec3 dir;
	};
}
