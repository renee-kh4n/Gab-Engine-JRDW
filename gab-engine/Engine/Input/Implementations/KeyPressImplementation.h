#pragma once

#include "../ActionImplementation.h"
#include "../Action/KeyPress.h"

namespace gde {
	namespace input {
		template<int K>
		class KeyPressImplementation : public ActionImplementation<KeyPress<K>> {
		public:
			// Inherited via ActionImplementation
            void UpdateState(Window* target)
            {
                auto key_state = target->GetKeyState(this->mState.key);

                if (key_state) {
                    if (this->mState.state == InputAction::State::END) {
                        this->mState.state = InputAction::State::START;
                        return;
                    }
                    else
                        this->mState.state = InputAction::State::WHILE;
                    return;
                }
                else{
                    this->mState.state = InputAction::State::END;
                    return;
                }
            }

            virtual void ResetState(Window* target) {}
		};
	}
}