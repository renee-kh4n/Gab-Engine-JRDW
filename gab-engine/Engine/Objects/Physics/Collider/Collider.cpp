#include "Collider.h"

gde::Collider::Collider(float radius)
{
	this->radius = radius;
}

float gde::Collider::GetWorldRadius()
{
	return this->radius;
}
