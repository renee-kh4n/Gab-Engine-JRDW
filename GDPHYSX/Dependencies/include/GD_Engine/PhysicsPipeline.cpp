#include "PhysicsPipeline.h"

void gde::PhysicsPipeline::Register(Object* object)
{
	this->registered_objects.push_back(object);
}

void gde::PhysicsPipeline::Update(double duration)
{
	for (auto object : this->registered_objects)
	{
		object->velocity += object->acceleration * duration;
		object->position = object->position + (object->velocity * duration) + ((object->acceleration * (duration * duration)) * (1.0f / 2.0f));

		object->UpdateDrawCall();
	}
}
