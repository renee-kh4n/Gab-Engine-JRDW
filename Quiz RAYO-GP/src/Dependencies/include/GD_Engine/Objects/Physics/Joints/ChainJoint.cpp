#include "ChainJoint.h"

void gde::ChainJoint::InvokeUpdate(float deltatime)
{
	auto delta = this->World()->position - this->to_rbody->World()->position;
	auto vel = this->to_rbody->velocity;


	if (delta.SqrMagnitude() < this->restLen * this->restLen)
		return;

	auto delta_dir = delta.Normalize();

	auto dot = vel.Dot(delta_dir);
	auto velChange = delta_dir * -dot;

	this->to_rbody->velocity += velChange;
}
