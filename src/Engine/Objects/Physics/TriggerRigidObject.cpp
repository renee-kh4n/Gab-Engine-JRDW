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

void gbe::TriggerRigidObject::Set_OnExit(std::function<void(PhysicsObject*)> newfunc)
{
	this->OnExit = newfunc;
}

void gbe::TriggerRigidObject::Set_OnStay(std::function<void(PhysicsObject*, float)> newfunc)
{
	this->OnStay = newfunc;
}

void gbe::TriggerRigidObject::InvokePhysicsUpdate(float deltatime)
{
	std::vector<PhysicsObject*> crap_thisframe = std::vector<PhysicsObject*>();

	for (int i = 0; i < this->GetTriggerRigidbody()->Get_numInside(); i++)
	{
		auto other = (*this->lookup_func)(this->GetTriggerRigidbody()->Get_inside(i));
		crap_thisframe.push_back(other);

		auto find = std::find(this->inside_last_Frame.begin(), this->inside_last_Frame.end(), other);

		if (find == this->inside_last_Frame.end()) {
			if (this->OnEnter)
				this->OnEnter(other);
		}
	}

	if (this->OnExit)
		for (auto smth : this->inside_last_Frame)
		{
			auto find_incurrent = std::find(crap_thisframe.begin(), crap_thisframe.end(), smth);

			if (find_incurrent == crap_thisframe.end())
				this->OnExit(smth);
		}

	this->inside_last_Frame.clear();

	for (auto other : crap_thisframe)
	{
		this->inside_last_Frame.push_back(other);

		if (this->OnStay)
			this->OnStay(other, deltatime);
	}
}

gbe::TriggerRigidObject::TriggerRigidObject()
{
	this->body = new physics::TriggerRigidBody(this);
}
