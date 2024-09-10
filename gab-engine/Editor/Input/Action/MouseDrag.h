#pragma once

#include "Editor/Input/InputAction.h"
#include "Datatypes/Vector2Int.h"

namespace gde {
	namespace input {
		template<int Trigger>
		struct MouseDrag : public InputAction {
			Vector2Int delta;
		};
	}
}