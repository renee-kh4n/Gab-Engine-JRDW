#include "MouseScrollImplementation.h"

void gde::input::MouseScrollImplementation::UpdateState(Window* target)
{
	/* TODO
	if (this->initialized_callback == false) {
		target->RegisterWindowCallback(GDE_CALLBACKID_MOUSESCROLL, [this](void* value) {
			this->mState.delta = *(Vector2*)value;
			this->mState.state = InputAction::State::START;
		});

		window_cache = target;
		glfwSetScrollCallback(target->Get_window(), [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* user = (Window*)glfwGetWindowUserPointer(window);
			user->InvokeWindowCallback(GDE_CALLBACKID_MOUSESCROLL, new Vector2(xoffset, yoffset));
		});

		this->initialized_callback = true;
	}
	*/
}

void gde::input::MouseScrollImplementation::ResetState(Window* target)
{
	if (this->mState.state == InputAction::State::START) {
		auto test = 0;
		test++;
	}

	this->mState.state = InputAction::State::END;
}
