#pragma once

#include "Engine/Input/InputAction.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace input {
		struct MouseScroll : public InputAction {
			Vector2 delta;
		};
	}
}