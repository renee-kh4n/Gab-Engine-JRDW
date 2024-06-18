#include "SpacebarPauser.h"

namespace gde {
	SpacebarPauser::SpacebarPauser(Time* toPause)
	{
		this->toPause = toPause;
	}

	void SpacebarPauser::OnInput(KeyPress<GLFW_KEY_SPACE>* value, bool changed)
	{
		if (value->state == input::InputAction::START && changed) {
			this->toPause->paused = !this->toPause->paused;
		}
	}
}