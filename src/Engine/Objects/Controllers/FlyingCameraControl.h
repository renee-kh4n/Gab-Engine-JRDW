#pragma once

#include "ControllerBase.h"

#include "Math/gbe_math.h"
#include "../Rendering/Camera.h"

namespace gbe {
	class FlyingCameraControl : public ControllerBase {
	private:
		Vector2 orbital_rotation;
		Camera* mCam;
	public:
		FlyingCameraControl();
		void OnEnterHierarchy(Object* other) override;
	};
}