#include "OrbitalControl.h"
#include <glm/ext.hpp>

void gbe::OrbitalControl::OnInput(WasdDelta* value, bool changed)
{
	if (value->state != WasdDelta::WHILE)
		return;

	this->orbital_rotation += value->delta;
	if (this->orbital_rotation.y < -70)
		this->orbital_rotation.y = -70;
	if (this->orbital_rotation.y > 70)
		this->orbital_rotation.y = 70;

	this->Local().rotation.Set(Quaternion::Euler(Vector3(orbital_rotation.x, orbital_rotation.y, 0)));
}
