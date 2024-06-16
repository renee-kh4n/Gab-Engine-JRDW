#pragma once

#include "InputAction.h"

namespace gde {
	namespace input {
		class ActionImplementation_base {
		public:
			virtual bool CheckStateChanged() = 0;
			virtual InputAction* GetState() = 0;
		};

		template<typename TAction>
		class ActionImplementation : public ActionImplementation_base {
		protected:
			TAction mState;
		public:
			virtual bool CheckStateChanged() override {
				auto old_state = mState;
				UpdateState();
				return mState.state != old_state.state;
			}
			virtual InputAction* GetState() override {
				return &mState;
			}
			virtual void UpdateState() = 0;
		};
	}
}