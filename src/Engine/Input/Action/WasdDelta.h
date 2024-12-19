#pragma once

#include "Engine/Input/InputAction.h"
#include "Math/Vector2.h"

namespace gbe {
	namespace input {
		struct WasdDelta : public InputAction {
			Vector2 delta;
		};
	}
}