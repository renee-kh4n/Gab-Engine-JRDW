#pragma once

#include "../InputAction.h"
#include "../../Datatypes/Vector2.h"

namespace gde {
	namespace input {
		template<int Trigger>
		struct MouseDrag : public InputAction {
			Vector2 delta;
		};
	}
}