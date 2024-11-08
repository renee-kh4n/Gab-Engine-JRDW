#include "MouseScrollImplementation.h"

void gde::input::MouseScrollImplementation::UpdateState(Window* target)
{
	
}

void gde::input::MouseScrollImplementation::ResetState(Window* target)
{
	this->mState.state = InputAction::State::END;
}
