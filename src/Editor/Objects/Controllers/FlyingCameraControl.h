#pragma once

#include "ControllerBase.h"

#include "../Datatypes/Vector2.h"
#include "../../Input/Action/MouseDrag.h"
#include "../../Input/Action/MouseScroll.h"
#include "Input/KeyDefines.h"
#include <iostream>

namespace gde {
	class FlyingCameraControl : public ControllerBase {
	private:
		Vector2 orbital_rotation;
	public:
		FlyingCameraControl();
	};
}