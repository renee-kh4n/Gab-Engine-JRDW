#pragma once

#include "Engine/Input/InputAction.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace input {
		template<int Trigger>
		struct MouseDrag : public InputAction {
			Vector2Int delta;
		};
	}
}