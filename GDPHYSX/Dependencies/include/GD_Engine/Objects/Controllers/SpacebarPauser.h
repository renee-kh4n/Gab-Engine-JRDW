#pragma once

#include "../Object.h"
#include "../../Time.h"
#include "InputCustomer.h"
#include "../../Input/Action/KeyPress.h"
#include "GLFW/glfw3.h"

namespace gde {
	class SpacebarPauser : public Object, public InputCustomer<KeyPress<GLFW_KEY_SPACE>>{
		Time* toPause;
	public:
		SpacebarPauser(Time* toPause);
		// Inherited via InputCustomer
		virtual void OnInput(KeyPress<GLFW_KEY_SPACE>* value, bool changed) override;
	};
}