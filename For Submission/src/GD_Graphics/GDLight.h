#pragma once

#include "GDutil.h"

struct GDPointLight {
	glm::vec3 color;
	float intensity;
	glm::vec3 pos;
};

struct GDConeLight {
	glm::vec3 color;
	float intensity;
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec2 angle;
};

struct GDDirLight {
	glm::vec3 color;
	float intensity;
	glm::vec3 dir;
};
