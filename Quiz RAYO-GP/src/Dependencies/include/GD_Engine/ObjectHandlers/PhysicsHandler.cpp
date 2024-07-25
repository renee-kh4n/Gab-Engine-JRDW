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

void gde::PhysicsHandler::AddContact(RigidObject* r1, RigidObject* r2, float restitution, float depth)
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
	GetOverlap();

	for (auto link : this->links)
	{
		auto contact = link->GetContact();

		if (contact != nullptr)
			contacts.push_back(contact);
	}

}

void gde::PhysicsHandler::GetOverlap()
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

			AddContact(*a, *b, 1.0f, sqrt(rad2 - delta2));
		}
	}
}
