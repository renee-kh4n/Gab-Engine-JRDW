#pragma once

#include "ControllerBase.h"

#include "Math/gbe_math.h"

namespace gbe {
	class FlyingCameraControl : public ControllerBase {
	private:
		Vector2 orbital_rotation;
	public:
		FlyingCameraControl();
	};
}