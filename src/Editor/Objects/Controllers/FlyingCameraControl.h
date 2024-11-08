#pragma once

#include "ControllerBase.h"

#include "../Datatypes/Vector2.h"

namespace gde {
	class FlyingCameraControl : public ControllerBase {
	private:
		Vector2 orbital_rotation;
	public:
		FlyingCameraControl();
	};
}