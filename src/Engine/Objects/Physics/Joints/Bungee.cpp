#include "Bungee.h"

void gbe::Bungee::InvokeUpdate(float deltatime)
{
	if (to_rbody == nullptr) return;

	Vector3 pos = to_rbody->World()->position;

	Vector3 force = pos - this->World()->position;
	float mag = force.Magnitude();
	float delta = mag - this->restLen;

	if (delta < 0)
		return;

	float springForce_mag = -springConst * abs(delta);

	auto force_dir = force * (1.0f / mag);
	auto spring_force = force_dir * springForce_mag;

	to_rbody->AddForce(spring_force);
}
