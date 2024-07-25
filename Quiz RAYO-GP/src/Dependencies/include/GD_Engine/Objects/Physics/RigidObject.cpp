#include "RigidObject.h"

gde::RigidObject::RigidObject() {
	this->mass = 1;
	this->frame_force = Vector3::zero;
	this->velocity = Vector3(0, 0, 0);
	this->acceleration = Vector3(0, 0, 0);
}

gde::RigidObject::~RigidObject()
{
}


void gde::RigidObject::AddForce(Vector3 force)
{
	this->frame_force += force;
}

void gde::RigidObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
}
