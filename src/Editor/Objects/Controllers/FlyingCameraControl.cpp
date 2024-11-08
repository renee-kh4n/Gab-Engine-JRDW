#include "FlyingCameraControl.h"

namespace gde {

	/*
	void gde::FlyingCameraControl::OnInput(MouseDrag<GLFW_MOUSE_BUTTON_2>* value, bool changed)
	{
		if (value->state != MouseDrag<GLFW_MOUSE_BUTTON_2>::WHILE)
			return;

		this->orbital_rotation += value->delta;
		if (this->orbital_rotation.y < -70)
			this->orbital_rotation.y = -70;
		if (this->orbital_rotation.y > 70)
			this->orbital_rotation.y = 70;

		this->SetRotation(Vector3(orbital_rotation.x, orbital_rotation.y, 0));
	}

	void gde::FlyingCameraControl::OnInput(MouseDrag<GLFW_MOUSE_BUTTON_3>* value, bool changed)
	{
		if (value->state != MouseDrag<GLFW_MOUSE_BUTTON_3>::WHILE)
			return;

		this->orbital_rotation += value->delta;
		if (this->orbital_rotation.y < -70)
			this->orbital_rotation.y = -70;
		if (this->orbital_rotation.y > 70)
			this->orbital_rotation.y = 70;

		this->SetRotation(Vector3(orbital_rotation.x, orbital_rotation.y, 0));
	}
	*/
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

			this->SetRotation(Vector3(orbital_rotation.y, orbital_rotation.x, 0));
		});

		this->inputreceivers.push_back(mouserightdrag);

		auto mousemiddledrag = new InputCustomer<MouseDrag<Keys::MOUSE_MIDDLE>>([this](MouseDrag<Keys::MOUSE_MIDDLE>* value, bool changed) {
			if (value->state != MouseDrag<Keys::MOUSE_MIDDLE>::WHILE)
				return;

			this->TranslateLocal(Vector3(value->delta.x, value->delta.y, 0) * 0.2f);
		});

		this->inputreceivers.push_back(mousemiddledrag);

		auto mousescroll = new InputCustomer<MouseScroll>([this](MouseScroll* value, bool changed) {
			if (value->state != InputAction::State::START)
			return;

		this->TranslateLocal(Vector3(0, 0, value->delta.y) * 4.f);
			});

		this->inputreceivers.push_back(mousescroll);
		
	}
}