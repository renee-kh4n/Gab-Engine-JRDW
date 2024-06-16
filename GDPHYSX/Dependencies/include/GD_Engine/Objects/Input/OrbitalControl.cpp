#include "OrbitalControl.h"

void gde::OrbitalControl::OnInput(MouseRightDrag* value, bool changed)
{
	this->Rotate(Vector3(value->delta.y, value->delta.x, 0));
}
