#include "CameraSwitcher.h"

gde::CameraSwitcher::CameraSwitcher(void* Camera1, void* Camera2, void** toAssign)
{
	this->Camera1 = Camera1;
	this->Camera2 = Camera2;
	this->toAssign = toAssign;
}

void gde::CameraSwitcher::OnInput(KeyPress<GLFW_KEY_Q>* value, bool changed)
{
	if (changed && value->state == input::InputAction::START) {
		if (*toAssign == Camera1) {
			*toAssign = Camera2;
			return;
		}
		*toAssign = Camera1;
	}
}
