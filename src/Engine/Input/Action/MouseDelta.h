#pragma once

#include "Engine/Input/InputAction.h"
#include "Math/Vector2.h"

namespace gbe {
	namespace input {
		struct MouseDelta : public InputAction {
			Vector2Int delta;
		};
	}
}