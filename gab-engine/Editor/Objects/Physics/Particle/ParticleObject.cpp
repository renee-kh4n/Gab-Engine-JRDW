#include "ParticleObject.h"

gde::ParticleClass::ParticleClass(float radius)
{
	this->radius = radius;
}

float gde::ParticleClass::GetRadius()
{
	return this->radius;
}
