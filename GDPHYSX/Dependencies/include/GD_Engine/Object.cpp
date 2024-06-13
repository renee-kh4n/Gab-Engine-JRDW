#include "Object.h"

gde::Object::Object()
{
	this->position = Vector3(0, 0, 0);
	this->scale = Vector3(1, 1, 1);
	this->rotation = Vector3(0, 0, 0);

	this->mass = 1;
	this->frame_impule_force = Vector3::zero;
	this->frame_continuous_force = Vector3::zero;
	this->velocity = Vector3(0, 0, 0);
	this->acceleration = Vector3(0, 0, 0);
}

void gde::Object::UpdateDrawCall()
{
	this->mDrawCall->pos = this->position;
	this->mDrawCall->scale = this->scale;
	this->mDrawCall->rot = this->rotation;
}

void gde::Object::AddForce(Vector3 force, bool continuous)
{
	if (continuous)
		this->frame_continuous_force += force;
	else
		this->frame_impule_force += force;
}

void gde::Object::Destroy()
{
	this->isDestroyed = true;
}

bool gde::Object::get_isDestroyed()
{
	return this->isDestroyed;
}
