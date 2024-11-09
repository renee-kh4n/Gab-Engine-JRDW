#include "ParticleObject.h"

gbe::ParticleClass::ParticleClass(float radius)
{
	this->radius = radius;
}

float gbe::ParticleClass::GetRadius()
{
	return this->radius;
}
