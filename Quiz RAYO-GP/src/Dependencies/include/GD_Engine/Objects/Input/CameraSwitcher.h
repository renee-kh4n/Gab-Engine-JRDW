#pragma once

#include "../Object.h"
#include "InputCustomer.h"
#include "../../Input/Action/KeyPress.h"
#include "GLFW/glfw3.h"

namespace gde {
	class CameraSwitcher : public Object, public InputCustomer<KeyPress<GLFW_KEY_Q>> {
	private:
		void* Camera1;
		void* Camera2;
		void** toAssign;
	public:
		CameraSwitcher(void* Camera1, void* Camera2, void** toAssign);

		// Inherited via InputCustomer
		virtual void OnInput(KeyPress<GLFW_KEY_Q>* value, bool changed) override;
	};
}