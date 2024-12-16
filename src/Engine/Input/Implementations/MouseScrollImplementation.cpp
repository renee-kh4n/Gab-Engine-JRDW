#include "MouseScrollImplementation.h"

void gbe::input::MouseScrollImplementation::UpdateState(Window* target)
{
    auto is_up = target->GetKeyState(gbe::Keys::MOUSE_SCROLL_UP);
    auto is_down = target->GetKeyState(gbe::Keys::MOUSE_SCROLL_DOWN);

    if (is_up || is_down) {
        if (this->mState.state == InputAction::State::END)
            this->mState.state = InputAction::State::START;
        else
            this->mState.state = InputAction::State::WHILE;

        this->mState.delta.y = is_up ? 1 : -1;

        return;
    }
    else {
        this->mState.state = InputAction::State::END;
        return;
    }
}

void gbe::input::MouseScrollImplementation::ResetState(Window* target)
{
	this->mState.state = InputAction::State::END;

    this->mState.delta = Vector2::zero;
}
