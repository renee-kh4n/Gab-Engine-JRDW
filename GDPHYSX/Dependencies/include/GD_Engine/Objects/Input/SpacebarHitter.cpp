#include "SpacebarHitter.h"

gde::SpacebarHitter::SpacebarHitter(RigidObject* toHit, Vector3 forceToHit)
{
	this->toHit = toHit;
	this->forceToHit = forceToHit;
}

void gde::SpacebarHitter::OnInput(KeyPress<GLFW_KEY_SPACE>* value, bool changed)
{
	if (value->state == input::InputAction::START && changed) {
		this->toHit->AddForce(this->forceToHit);
	}
}
