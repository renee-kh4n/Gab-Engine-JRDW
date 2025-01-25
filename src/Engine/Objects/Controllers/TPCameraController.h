#pragma once

#include "ControllerBase.h"

#include "Math/gbe_math.h"

namespace gbe {
	class TPCameraController : public ControllerBase {
	private:
		Vector2 orbital_rotation;
		Object* pivot;
	public:
		void Set_pivot(Object* other);
		TPCameraController();
	};
}