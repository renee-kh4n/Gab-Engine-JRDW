#include "PhysicsHandler.h"

#include <iostream>

gbe::PhysicsHandler::PhysicsHandler()
{
	this->subhandlers.push_back(&this->forcevolume_handler);
}

void gbe::PhysicsHandler::Update(double duration)
{
	for (auto ro : this->object_list) {

		//Translational
		for (auto volume : this->forcevolume_handler.object_list) {
			volume->TryApply(ro);
		}

		auto total_force = ro->frame_force;

		ro->acceleration = Vector3::zero;
		ro->acceleration += total_force * (1 / ro->mass);

		float dur_f = (float)duration;

		ro->TranslateWorld((ro->velocity * dur_f) + ((ro->acceleration * (dur_f * dur_f)) * (1.0f / 2.0f)));

		ro->velocity += ro->acceleration * dur_f;
		ro->velocity *= powf(ro->damping, dur_f);

		ro->frame_force = Vector3::zero;

		//Angular
		float mI = ro->MomentOfInertia();
		ro->angularVelocity += ro->accumulatedTorque;
		ro->angularVelocity *= duration * ((float)1 / mI);
		ro->angularVelocity *= powf(ro->amgularDamp, duration);

		auto aV = ro->angularVelocity;
		aV *= duration;
		float aVmag = aV.Magnitude();
		Vector3 aVdir = aV * (1.0f / aVmag);

		if (aVmag > 0.0001f) {
			ro->Rotate(aVdir, aVmag);
		}

		ro->accumulatedTorque = Vector3::zero;
	}

	this->GenerateContacts();

	if (this->contacts.size() > 0)
		this->mContactResolver.ResolveContacts(contacts, duration);
}

void gbe::PhysicsHandler::AddContact(RigidObject* r1, RigidObject* r2, float restitution, float depth)
{
	CollisionContact* toAdd = new CollisionContact();
	auto delta = (r1->World()->position - r2->World()->position);
	auto delta_mag = delta.length();

	toAdd->objects[0] = r1;
	toAdd->objects[1] = r2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = delta * (1.0f / delta_mag);
	//toAdd->depth = delta_mag;

	contacts.push_back(toAdd);
}

void gbe::PhysicsHandler::AddLink(RigidObjectLink* link)
{
	this->links.push_back(link);
}

void gbe::PhysicsHandler::GenerateContacts()
{
	contacts.clear();
	GetOverlap();

	for (auto link : this->links)
	{
		auto contact = link->GetContact();

		if (contact != nullptr)
			contacts.push_back(contact);
	}

}

void gbe::PhysicsHandler::GetOverlap()
{
	for (size_t i = 0; i < this->object_list.size() - 1; i++)
	{
		std::list<RigidObject*>::iterator a = std::next(object_list.begin(), i);

		for (size_t j = i + 1; j < this->object_list.size(); j++)
		{
			std::list<RigidObject*>::iterator b = std::next(object_list.begin(), j);

			Vector3 delta = (*a)->World()->position - (*b)->World()->position;
			float rad_delta = (*a)->colliders.front()->GetWorldRadius() + (*b)->colliders.front()->GetWorldRadius();
			float rad2 = rad_delta * rad_delta;
			float delta2 = delta.SqrMagnitude();

			if (delta2 > rad2)
				continue;

			AddContact(*a, *b, 0.9f, sqrt(rad2 - delta2));
		}
	}
}
