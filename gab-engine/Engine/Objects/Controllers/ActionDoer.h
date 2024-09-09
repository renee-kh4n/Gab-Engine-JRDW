#pragma once

#include "../Object.h"
#include "InputCustomer.h"
#include "../../Input/Action/KeyPress.h"
#include "GLFW/glfw3.h"
#include <functional>

namespace gde {
	template<int K>
	class ActionDoer : public Object, public InputCustomer<KeyPress<K>> {
	private:
		std::function<void()> action;
	public:
		gde::ActionDoer<K>(std::function<void()> action)
		{
			this->action = action;
		}

		// Inherited via InputCustomer
		virtual void OnInput(KeyPress<K>* value, bool changed) override {
			if (changed && value->state == input::InputAction::START) {
				this->action();
			}
		}
	};
}