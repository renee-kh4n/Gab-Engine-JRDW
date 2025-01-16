#include "RigidObject.h"
#include "RigidObject.h"
#include "RigidObject.h"
#include "RigidObject.h"

gbe::RigidObject::RigidObject(bool is_static) {
	this->body = new physics::Rigidbody(is_static);
}

gbe::RigidObject::~RigidObject()
{
}

gbe::physics::Rigidbody* gbe::RigidObject::GetRigidbody()
{
	if (this->body == nullptr)
		throw "Null rigidbody";

	return static_cast<physics::Rigidbody*>(this->body);
}
