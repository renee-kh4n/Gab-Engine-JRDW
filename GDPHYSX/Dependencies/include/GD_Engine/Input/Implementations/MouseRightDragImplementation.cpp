#include "MouseRightDragImplementation.h"
#include "GLFW/glfw3.h"

void gde::input::MouseRightDragImplementation::UpdateState(Window* target)
{
    auto key_state = glfwGetMouseButton(target->window, GLFW_MOUSE_BUTTON_RIGHT);

    if (key_state == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(target->window, &x, &y);
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
