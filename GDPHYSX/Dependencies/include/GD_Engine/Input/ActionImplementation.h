#pragma once

#include "InputAction.h"

namespace gde {
	namespace input {
		class ActionImplementation_base {
		public:
			virtual void UpdateState(InputAction* action) = 0;
		};

		template<typename TAction>
		class ActionImplementation : public ActionImplementation_base {
		protected:
			TAction mState;
		public:
			virtual void UpdateState(InputAction* action) override {
				*action = mState;
			}
			virtual void UpdateState() = 0;
		};
	}
}