#include "OrbitalControl.h"
#include <glm/ext.hpp>

namespace gde {
	void OrbitalControl::OnInput(WasdDelta* value, bool changed)
	{
		if (value->state != WasdDelta::WHILE)
			return;

		this->orbital_rotation += value->delta;
		if (this->orbital_rotation.y < -70)
			this->orbital_rotation.y = -70;
		if (this->orbital_rotation.y > 70)
			this->orbital_rotation.y = 70;

		this->transform = glm::mat4(1.0f);

		this->Rotate(this->World()->Right, this->orbital_rotation.y);
		this->Rotate(Vector3(0, 1, 0), -this->orbital_rotation.x);

		auto forward = this->World()->Forward;
		auto correct_right = Vector3(0, 1, 0).Cross(forward).Normalize();
		auto correct_up = forward.Cross(correct_right).Normalize();
		this->Orient(forward, correct_up);
	}
}