#pragma once

#include "../InputAction.h"
#include "../../Datatypes/Vector2.h"

namespace gde {
	namespace input {
		struct MouseRightDrag : public InputAction {
			Vector2 delta;
		};
	}
}