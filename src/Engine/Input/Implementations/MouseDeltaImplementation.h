#pragma once

#include "Input/ActionImplementation.h"
#include "Input/Action/MouseDelta.h"
#include "../Window/Window.h"

namespace gbe {
	namespace input {
		class MouseDeltaImplementation : public ActionImplementation<MouseDelta> {
		private:
			Vector2Int old_mouse_pos;
			// Inherited via ActionImplementation
			virtual void UpdateState(Window* target)
			{
				auto new_pos = target->GetMousePixelPos();
				this->mState.delta = new_pos - old_mouse_pos;
				old_mouse_pos = new_pos;

				if (this->mState.delta.length() > 0.1f) {
					if (this->mState.state == InputAction::State::END)
						this->mState.state = InputAction::State::START;
					else
						this->mState.state = InputAction::State::WHILE;

				}
				else {
					this->mState.delta = Vector2Int(0, 0);
					this->mState.state = InputAction::State::END;
					return;
				}
			}


			virtual void ResetState(Window* target) {}
		};
	}
}