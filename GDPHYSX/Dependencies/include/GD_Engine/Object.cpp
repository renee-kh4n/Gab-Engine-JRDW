#include "Object.h"

gde::Object::Object()
{
	this->position = Vector3(0, 0, 0);
	this->scale = Vector3(1, 1, 1);
	this->rotation = Vector3(0, 0, 0);

	this->velocity = Vector3(0, 0, 0);
	this->acceleration = Vector3(0, 0, 0);
}

void gde::Object::UpdateDrawCall()
{
	this->mDrawCall->pos = this->position;
	this->mDrawCall->scale = this->scale;
	this->mDrawCall->rot = this->rotation;
}

void gde::Object::Destroy()
{
	this->isDestroyed = true;
}

bool gde::Object::get_isDestroyed()
{
	return this->isDestroyed;
}
