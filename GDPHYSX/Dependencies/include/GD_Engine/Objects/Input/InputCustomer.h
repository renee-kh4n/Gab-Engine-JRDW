#pragma once

#include "../../Input/InputAction.h"
#include "../Object.h"

namespace gde {
	using namespace input;

	class InputCustomer_base : public Object {
		virtual bool TryReceive(InputAction value, InputAction::State state) = 0;
	};

	template<typename TInput>
	class InputCustomer : public InputCustomer_base {
	protected:
		virtual void OnInput(TInput* value, InputAction::State state) = 0;
	public:
		virtual bool TryReceive(InputAction value, InputAction::State state) override {
			auto action_cast = dynamic_cast<TInput*>(&value);

			if (action_cast == nullptr)
				return false;

			this->OnInput(action_cast, state);

			return true;
		}
	};
}