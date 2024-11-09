#include "MouseScrollImplementation.h"

void gbe::input::MouseScrollImplementation::UpdateState(Window* target)
{
	
}

void gbe::input::MouseScrollImplementation::ResetState(Window* target)
{
	this->mState.state = InputAction::State::END;
}
