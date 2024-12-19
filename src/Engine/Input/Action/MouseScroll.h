#pragma once

#include "Input/InputAction.h"
#include "../Math/Vector2.h"

namespace gbe {
	namespace input {
		struct MouseScroll : public InputAction {
			Vector2 delta;
		};
	}
}