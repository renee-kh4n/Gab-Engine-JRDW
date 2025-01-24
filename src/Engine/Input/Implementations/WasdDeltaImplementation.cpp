#include "WasdDeltaImplementation.h"
#include "../KeyDefines.h"

void gbe::input::WasdDeltaImplementation::UpdateState(Window* target)
{
    auto w_state = target->GetKeyState(gbe::Keys::W);
    auto a_state = target->GetKeyState(gbe::Keys::A);
    auto s_state = target->GetKeyState(gbe::Keys::S);
    auto d_state = target->GetKeyState(gbe::Keys::D);

    auto wasd_any = w_state || a_state || s_state || d_state;

    if (wasd_any) {
        if (this->mState.state == InputAction::State::END) {
            this->mState.state = InputAction::State::START;
            return;
        }
        else
            this->mState.state = InputAction::State::WHILE;

        this->mState.delta.x = a_state ? -1 : d_state ? 1 : 0;
        this->mState.delta.y = s_state ? -1 : w_state ? 1 : 0;

        return;
    }
    else {
        this->mState.state = InputAction::State::END;

        this->mState.delta = Vector2::zero;

        return;
    }
}

void gbe::input::WasdDeltaImplementation::ResetState(Window* target)
{

}
