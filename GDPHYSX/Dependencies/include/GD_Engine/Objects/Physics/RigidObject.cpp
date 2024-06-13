#include "RigidObject.h"

gde::RigidObject::RigidObject() {
	this->mass = 1;
	this->frame_impule_force = Vector3::zero;
	this->frame_continuous_force = Vector3::zero;
	this->velocity = Vector3(0, 0, 0);
	this->acceleration = Vector3(0, 0, 0);
}

gde::RigidObject::~RigidObject()
{
}


void gde::RigidObject::AddForce(Vector3 force, bool continuous)
{
	if (continuous)
		this->frame_continuous_force += force;
	else
		this->frame_impule_force += force;
}
