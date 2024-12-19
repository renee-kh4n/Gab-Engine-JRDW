#include "FlyingCameraControl.h"

#include "Engine/Input/Action/MouseDrag.h"
#include "Engine/Input/Action/MouseScroll.h"
#include "Engine/Input/KeyDefines.h"

namespace gbe {
	FlyingCameraControl::FlyingCameraControl()
	{
		auto mouserightdrag = new InputCustomer<MouseDrag<Keys::MOUSE_RIGHT>>([this](MouseDrag<Keys::MOUSE_RIGHT>* value, bool changed) {
			if (value->state != MouseDrag<Keys::MOUSE_RIGHT>::WHILE)
				return;

			Vector2 drag_delta = value->delta;
			drag_delta *= 0.5f;

			this->orbital_rotation.x -= drag_delta.x;
			this->orbital_rotation.y += drag_delta.y;
			if (this->orbital_rotation.y < -70)
				this->orbital_rotation.y = -70;
			if (this->orbital_rotation.y > 70)
				this->orbital_rotation.y = 70;

			this->Local().rotation.Set(Quaternion::Euler(Vector3(orbital_rotation.y, orbital_rotation.x, 0)));
		});

		this->inputreceivers.push_back(mouserightdrag);

		auto mousemiddledrag = new InputCustomer<MouseDrag<Keys::MOUSE_MIDDLE>>([this](MouseDrag<Keys::MOUSE_MIDDLE>* value, bool changed) {
			if (value->state != MouseDrag<Keys::MOUSE_MIDDLE>::WHILE)
				return;

			auto sensitivity = 0.2f;
			this->Local().position.Set(this->Local().position.Get() + (this->Local().GetUp() * (value->delta.y * sensitivity)));
			this->Local().position.Set(this->Local().position.Get() + (this->Local().GetRight() * (value->delta.x * sensitivity)));
		});

		this->inputreceivers.push_back(mousemiddledrag);

		auto mousescroll = new InputCustomer<MouseScroll>([this](MouseScroll* value, bool changed) {
			if (value->state != InputAction::State::START)
				return;

			auto delta = this->World().GetForward() * (value->delta.y * 1.f);
			this->Local().position.Set(this->Local().position.Get() + delta);
			});

		this->inputreceivers.push_back(mousescroll);
		
	}
}