#include "Collider.h"

gbe::Collider::Collider(float radius)
{
	this->radius = radius;
}

float gbe::Collider::GetWorldRadius()
{
	return this->radius;
}
