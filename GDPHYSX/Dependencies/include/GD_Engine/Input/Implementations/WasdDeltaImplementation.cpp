#include "WasdDeltaImplementation.h"

void gde::input::WasdDeltaImplementation::UpdateState(Window* target)
{
    auto w_state = glfwGetKey(target->window, GLFW_KEY_W);
    auto a_state = glfwGetKey(target->window, GLFW_KEY_A);
    auto s_state = glfwGetKey(target->window, GLFW_KEY_S);
    auto d_state = glfwGetKey(target->window, GLFW_KEY_D);

    if (w_state == GLFW_PRESS || a_state == GLFW_PRESS || s_state == GLFW_PRESS || d_state == GLFW_PRESS) {
        if (this->mState.state == InputAction::State::END) {
            this->mState.state = InputAction::State::START;
        }
        else
            this->mState.state = InputAction::State::WHILE;

        this->mState.delta = Vector2(0, 0);

        if (w_state == GLFW_PRESS)
            this->mState.delta.y += 1;
        if (a_state == GLFW_PRESS)
            this->mState.delta.x -= 1;
        if (s_state == GLFW_PRESS)
            this->mState.delta.y -= 1;
        if (d_state == GLFW_PRESS)
            this->mState.delta.x += 1;

        return;
    }
    if (w_state == GLFW_RELEASE || a_state == GLFW_RELEASE || s_state == GLFW_RELEASE || d_state == GLFW_RELEASE) {
        this->mState.delta = Vector2::zero;
        this->mState.state = InputAction::State::END;
        return;
    }
}
