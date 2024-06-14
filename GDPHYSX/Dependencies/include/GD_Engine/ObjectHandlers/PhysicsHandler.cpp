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

		rigidobject->Translate((rigidobject->velocity * duration) + ((rigidobject->acceleration * (duration * duration)) * (1.0f / 2.0f)));
		
		rigidobject->velocity += rigidobject->acceleration * duration;
		rigidobject->velocity *= powf(rigidobject->damping, duration);

		rigidobject->frame_force = Vector3::zero;
	}
}
