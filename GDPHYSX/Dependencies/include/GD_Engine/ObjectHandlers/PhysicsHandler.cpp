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

	this->mContactResolver.ResolveContacts(contacts, duration);
}

void gde::PhysicsHandler::AddContact(RigidObject* r1, RigidObject* r2, float restitution, Vector3 contactNormal)
{
	CollisionContact* toAdd = new CollisionContact();

	toAdd->objects[0] = r1;
	toAdd->objects[1] = r2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;

	contacts.push_back(toAdd);
}
