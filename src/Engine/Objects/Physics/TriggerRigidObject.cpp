#include "TriggerRigidObject.h"

gbe::physics::TriggerRigidBody* gbe::TriggerRigidObject::GetTriggerRigidbody()
{
	if (this->body == nullptr)
		throw "Null rigidbody";

	return static_cast<gbe::physics::TriggerRigidBody*>(this->body);
}

void gbe::TriggerRigidObject::Set_OnEnter(std::function<void(PhysicsObject*)> newfunc)
{
	this->OnEnter = newfunc;
}

void gbe::TriggerRigidObject::InvokePhysicsUpdate(float deltatime)
{
	for (int i = 0; i < this->GetTriggerRigidbody()->Get_numInside(); i++)
	{
		auto other = (*this->lookup_func)(this->GetTriggerRigidbody()->Get_inside(i));

		this->OnEnter(other);
	}
}

gbe::TriggerRigidObject::TriggerRigidObject()
{
	this->body = new physics::TriggerRigidBody();
}
