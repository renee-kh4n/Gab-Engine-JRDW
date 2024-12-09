#include "RigidObject.h"

gbe::RigidObject::RigidObject() {
}

gbe::RigidObject::~RigidObject()
{
}

float gbe::RigidObject::Get_mass()
{
	return 1.0f;
}

void gbe::RigidObject::AddForce(Vector3 force)
{
}

void gbe::RigidObject::AddForceAtPoint(Vector3 force, Vector3 relativeWorldPoint)
{
}

void gbe::RigidObject::Set_velocity(Vector3)
{
}

gbe::Vector3 gbe::RigidObject::Get_velocity()
{
	return Vector3();
}

void gbe::RigidObject::Set_angularVelocity(Vector3)
{
}

gbe::Vector3 gbe::RigidObject::Get_angularVelocity()
{
	return Vector3();
}

void gbe::RigidObject::OnEnterHierarchy(Object* newChild)
{
	Object::OnEnterHierarchy(newChild);

	auto col = dynamic_cast<Collider*>(newChild);

	if (col == nullptr)
		return;

	this->colliders.push_back(col);
}
