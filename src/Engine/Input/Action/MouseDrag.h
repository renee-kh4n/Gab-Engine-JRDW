#pragma once

#include "Input/InputAction.h"
#include "../Datatypes/Vector2Int.h"

namespace gbe {
	namespace input {
		template<int Trigger>
		struct MouseDrag : public InputAction {
			Vector2Int delta;
		};
	}
}