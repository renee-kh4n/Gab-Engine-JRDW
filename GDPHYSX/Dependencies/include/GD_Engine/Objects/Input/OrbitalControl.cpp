#include "OrbitalControl.h"

void gde::OrbitalControl::OnInput(MouseRightDrag* value, InputAction::State state)
{
	this->Rotate(Vector3(value->delta.y, value->delta.x, 0));
}
