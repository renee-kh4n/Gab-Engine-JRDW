#pragma once

#include "GDutil.h"

struct GDPointLight {
	glm::vec3 color;
	float intensity;
	glm::vec3 pos;
};

struct GDDirLight {
	glm::vec3 color;
	glm::vec3 dir;
};
