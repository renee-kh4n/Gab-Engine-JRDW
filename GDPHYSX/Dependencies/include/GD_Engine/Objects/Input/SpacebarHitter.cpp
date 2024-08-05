#include "SpacebarHitter.h"

gde::SpacebarHitter::SpacebarHitter(RigidObject* toHit, Vector3 forceToHit, Vector3 positionToHit)
{
	this->toHit = toHit;
	this->forceToHit = forceToHit;
	this->positionToHit = positionToHit;
}

void gde::SpacebarHitter::OnInput(KeyPress<GLFW_KEY_SPACE>* value, bool changed)
{
	if (value->state == input::InputAction::START && changed) {
		this->toHit->AddForceAtPoint(this->forceToHit, this->positionToHit);
	}
}
