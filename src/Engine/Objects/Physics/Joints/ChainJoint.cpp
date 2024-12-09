#include "ChainJoint.h"

void gbe::ChainJoint::InvokeUpdate(float deltatime)
{
	Vector3 delta = this->World()->position - this->to_rbody->World()->position;
	auto vel = this->to_rbody->body.Get_velocity();

	if (delta.SqrMagnitude() < this->restLen * this->restLen)
		return;

	Vector3 delta_dir = delta.Normalize();

	auto dot = vel.Dot(delta_dir);
	auto velChange = delta_dir * -dot;

	this->to_rbody->body.Set_velocity(vel + velChange);

	if (delta.SqrMagnitude() > this->restLen * this->restLen)
		this->to_rbody->SetPosition(this->World()->position + (-delta.Normalize() * restLen));
}
