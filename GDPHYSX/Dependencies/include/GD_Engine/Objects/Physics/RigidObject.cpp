#include "RigidObject.h"

using namespace gde;

RigidObject::RigidObject() {
	this->mass = 1;
	this->frame_force = Vector3::zero;
	this->velocity = Vector3(0, 0, 0);
	this->acceleration = Vector3(0, 0, 0);
}

RigidObject::~RigidObject()
{
}


void RigidObject::AddForce(Vector3 force)
{
	this->frame_force += force;
}
