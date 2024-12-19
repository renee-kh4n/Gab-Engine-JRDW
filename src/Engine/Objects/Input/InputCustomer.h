#pragma once

#include "Engine/Input/InputAction.h"
#include "Engine/Objects/Object.h"

namespace gbe {
	using namespace input;

	class InputCustomer_base {
	public:
		virtual bool TryReceive(InputAction* value, bool changed) = 0;
	};

	template<typename TInput>
	class InputCustomer : public InputCustomer_base {
	private:
		std::function<void(TInput*, bool)> OnInput;
	public:
		InputCustomer(std::function<void(TInput*, bool)> func) {
			this->OnInput = func;
		}

		virtual bool TryReceive(InputAction* value, bool changed) override {
			auto action_cast = dynamic_cast<TInput*>(value);

			if (action_cast == nullptr)
				return false;

			this->OnInput(action_cast, changed);

			return true;
		}
	};
}