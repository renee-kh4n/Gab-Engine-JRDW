#pragma once

#include "../ActionImplementation.h"
#include "../Action/MouseDrag.h"

namespace gde {
	namespace input {
		template<int Trigger>
		class MouseDragImplementation : public ActionImplementation<MouseDrag<Trigger>> {
		private:
			Vector2 old_mouse_pos;
			// Inherited via ActionImplementation
			virtual void UpdateState(Window* target)
            {
                auto key_state = glfwGetMouseButton(target->Get_window(), Trigger);

                if (key_state == GLFW_PRESS) {
                    double x, y;
                    glfwGetCursorPos(target->Get_window(), &x, &y);
                    auto new_pos = Vector2((float)x, (float)y);

                    if (this->mState.state == InputAction::State::END) {
                        this->mState.state = InputAction::State::START;
                        old_mouse_pos = new_pos;
                        return;
                    }
                    else
                        this->mState.state = InputAction::State::WHILE;

                    this->mState.delta = new_pos - old_mouse_pos;
                    old_mouse_pos = new_pos;

                    return;
                }
                if (key_state == GLFW_RELEASE) {
                    this->mState.delta = Vector2::zero;
                    this->mState.state = InputAction::State::END;
                    return;
                }
            }
		};
	}
}