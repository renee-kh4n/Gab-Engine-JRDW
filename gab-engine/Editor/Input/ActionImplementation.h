#pragma once

#include "InputAction.h"

namespace gde {

	class Window;

	namespace input {
		class ActionImplementation_base {
		public:
			virtual bool CheckStateChanged(Window* target) = 0;
			virtual InputAction* GetState() = 0;
			virtual void ResetState(Window* target) = 0;
		};

		template<typename TAction>
		class ActionImplementation : public ActionImplementation_base {
		protected:
			TAction mState;
		public:
			virtual bool CheckStateChanged(Window* target) override {
				auto old_state = mState;
				UpdateState(target);
				return mState.state != old_state.state;
			}
			virtual InputAction* GetState() override {
				return &mState;
			}
			virtual void UpdateState(Window* target) = 0;
		};
	}
}