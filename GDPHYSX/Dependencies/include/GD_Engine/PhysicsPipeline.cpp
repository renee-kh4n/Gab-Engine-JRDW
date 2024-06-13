#include "PhysicsPipeline.h"

void gde::PhysicsPipeline::Register(RigidObject* object)
{
	this->registered_objects.push_back(object);
}

void gde::PhysicsPipeline::Update(double duration)
{
	this->registered_objects.remove_if([](RigidObject* obj) {
		return obj->get_isDestroyed();
		});

	for (auto object : this->registered_objects)
	{
		object->acceleration = Vector3::zero;
		object->acceleration += object->frame_impule_force * (1/object->mass);
		object->acceleration += object->frame_continuous_force * (duration) * (1/object->mass);

		object->velocity += object->acceleration * duration;
		object->velocity = object->velocity * powf(object->damping, duration);

		object->Translate((object->velocity * duration) + ((object->acceleration * (duration * duration)) * (1.0f / 2.0f)));
		
		object->frame_impule_force = Vector3::zero;
		object->frame_continuous_force = Vector3::zero;
	}
}
