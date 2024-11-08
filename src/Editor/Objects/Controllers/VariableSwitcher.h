#pragma once

#include "../Object.h"
#include "InputCustomer.h"
#include "../../Input/Action/KeyPress.h"
#include "GLFW/glfw3.h"

namespace gde {
	template<int K>
	class VariableSwitcher : public Object, public InputCustomer<KeyPress<K>> {
	private:
		void* valueToAssign;
		void** targetToAssign;
	public:
		gde::VariableSwitcher<K>(void* valueToAssign, void** targetToAssign)
		{
			this->valueToAssign = valueToAssign;
			this->targetToAssign = targetToAssign;
		}

		virtual void OnInput(KeyPress<K>* value, bool changed) {
			if (changed && value->state == input::InputAction::START) {
				*targetToAssign = valueToAssign;
			}
		}
	};
}