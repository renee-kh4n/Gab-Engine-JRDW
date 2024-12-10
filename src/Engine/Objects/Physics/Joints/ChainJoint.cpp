#include "ChainJoint.h"

void gbe::ChainJoint::InvokeUpdate(float deltatime)
{
	Vector3 delta = this->to_rbody->World()->position - this->World()->position;

	if (delta.SqrMagnitude() < this->restLen * this->restLen)
		return;

	float delta_mag = delta.Magnitude();
	Vector3 delta_dir = delta / delta_mag;
	
	Vector3 velChange = Vector3();

	//cancel out outwards velocity
	auto vel = this->to_rbody->body.Get_velocity();
	float projection_len = vel.Dot(delta_dir);
	Vector3 projection_vec = delta_dir * projection_len;
	Vector3 cancelout = -projection_vec;

	//bring back vel
	Vector3 bringback = -(delta_mag - this->restLen) * delta_dir;

	this->to_rbody->body.Set_velocity((physics::PhysicsVector3)(vel + cancelout + bringback));
}
