#include "ForceVolume.h"

#include <iostream>

void gde::ForceVolume::TryApply(RigidObject* object)
{
	if (this->shape == SPHERE)
		if ((object->World()->position - this->World()->position).SqrMagnitude() > this->radius * this->radius)
			return;
	if (this->shape == BOX) {
		auto delta = object->World()->position - this->World()->position;
		if (abs(delta.x) > this->half_bounds.x)
			return;
		if (abs(delta.y) > this->half_bounds.y)
			return;
	}

	auto final_dir = Vector3::zero;

	if (this->mode == DIRECTIONAL)
		final_dir = this->vector;

	auto final_force = final_dir;

	if (this->forceMode == VELOCITY)
		final_force *= object->mass;

	object->AddForce(final_force);
}
