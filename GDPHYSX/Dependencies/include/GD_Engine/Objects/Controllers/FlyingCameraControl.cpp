#include "FlyingCameraControl.h"
#include "../../Input/Action/MouseDrag.h"

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

		auto mouserightdrag = new InputCustomer<MouseDrag<GLFW_MOUSE_BUTTON_2>>([this](MouseDrag<GLFW_MOUSE_BUTTON_2>* value, bool changed) {
			if (value->state != MouseDrag<GLFW_MOUSE_BUTTON_2>::WHILE)
			return;

		this->orbital_rotation += value->delta;
		if (this->orbital_rotation.y < -70)
			this->orbital_rotation.y = -70;
		if (this->orbital_rotation.y > 70)
			this->orbital_rotation.y = 70;

		this->SetRotation(Vector3(orbital_rotation.x, orbital_rotation.y, 0));
			});

		this->inputreceivers.push_back(mouserightdrag);
	}
}