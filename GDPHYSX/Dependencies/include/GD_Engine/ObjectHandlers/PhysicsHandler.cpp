#include "PhysicsHandler.h"

#include <iostream>

gde::PhysicsHandler::PhysicsHandler()
{
	this->subhandlers.push_back(&this->forcevolume_handler);
}

void gde::PhysicsHandler::Update(double duration)
{
	for (auto rigidobject : this->object_list) {

		for (auto volume : this->forcevolume_handler.object_list) {
			volume->TryApply(rigidobject);
		}

		auto total_force = rigidobject->frame_force;

		rigidobject->acceleration = Vector3::zero;
		rigidobject->acceleration += total_force * (1 / rigidobject->mass);

		float dur_f = (float)duration;

		rigidobject->TranslateWorld((rigidobject->velocity * dur_f) + ((rigidobject->acceleration * (dur_f * dur_f)) * (1.0f / 2.0f)));

		rigidobject->velocity += rigidobject->acceleration * dur_f;
		rigidobject->velocity *= powf(rigidobject->damping, dur_f);

		rigidobject->frame_force = Vector3::zero;
	}

	this->GenerateContacts();

	if (this->contacts.size() > 0)
		this->mContactResolver.ResolveContacts(contacts, duration);
}

void gde::PhysicsHandler::AddContact(RigidObject* r1, RigidObject* r2, float restitution)
{
	CollisionContact* toAdd = new CollisionContact();
	auto delta = (r1->World()->position - r2->World()->position);
	auto delta_mag = delta.Magnitude();

	toAdd->objects[0] = r1;
	toAdd->objects[1] = r2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = delta * (1.0f / delta_mag);
	//toAdd->depth = delta_mag;

	contacts.push_back(toAdd);
}

void gde::PhysicsHandler::AddLink(RigidObjectLink* link)
{
	this->links.push_back(link);
}

void gde::PhysicsHandler::GenerateContacts()
{
	contacts.clear();

	for (auto link : this->links)
	{
		auto contact = link->GetContact();

		if (contact != nullptr)
			contacts.push_back(contact);
	}
}
