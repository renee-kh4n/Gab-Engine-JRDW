#pragma once

#include "ControllerBase.h"

namespace gde {
	class FlyingCameraControl : public ControllerBase {
	private:
		Vector2 orbital_rotation;
	public:
		FlyingCameraControl();
	};
}