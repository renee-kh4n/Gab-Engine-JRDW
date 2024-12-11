#include "Bungee.h"

void gbe::Bungee::InvokeUpdate(float deltatime)
{
	if (to_rbody == nullptr) return;

	Vector3 pos = to_rbody->World().position.Get();

	Vector3 force = pos - this->World().position.Get();
	float mag = force.Magnitude();
	float delta = mag - this->restLen;

	if (delta < 0)
		return;

	float springForce_mag = -springConst * abs(delta);

	auto force_dir = force * (1.0f / mag);
	auto spring_force = force_dir * springForce_mag;

	to_rbody->body.AddForce((physics::PhysicsVector3)spring_force);
}
