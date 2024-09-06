#include "MouseScrollImplementation.h"

void gde::input::MouseScrollImplementation::UpdateState(Window* target)
{
	if (this->initialized_callback == false) {
		target->RegisterWindowCallback(GDE_CALLBACKID_MOUSESCROLL, [this](void* value) {
			this->ontime_delta = *(Vector2*)value;
		});

		window_cache = target;
		glfwSetScrollCallback(target->Get_window(), [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* user = (Window*)glfwGetWindowUserPointer(window);
			user->InvokeWindowCallback(GDE_CALLBACKID_MOUSESCROLL, new Vector2(xoffset, yoffset));
		});
	}

	if ((this->ontime_delta - this->late_delta).SqrMagnitude() > 0.01f) {
		this->late_delta = this->ontime_delta;

		if (this->mState.state == InputAction::State::END) {
			this->mState.state = InputAction::State::START;
			return;
		}
		else
			this->mState.state = InputAction::State::WHILE;
		return;
	}
	else {
		this->mState.state = InputAction::State::END;
		return;
	}
}
