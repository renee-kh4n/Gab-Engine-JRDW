#pragma once

#include "../../Input/InputAction.h"
#include "../Object.h"

namespace gde {
	using namespace input;

	class InputCustomer_base {
	public:
		virtual bool TryReceive(InputAction* value, bool changed) = 0;
	};

	template<typename TInput>
	class InputCustomer : public InputCustomer_base {
	protected:
		virtual void OnInput(TInput* value, bool changed) = 0;
	public:
		virtual bool TryReceive(InputAction* value, bool changed) override {
			auto action_cast = dynamic_cast<TInput*>(value);

			if (action_cast == nullptr)
				return false;

			this->OnInput(action_cast, changed);

			return true;
		}
	};
}