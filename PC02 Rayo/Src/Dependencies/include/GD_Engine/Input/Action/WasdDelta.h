#pragma once

#include "../InputAction.h"
#include "../../Datatypes/Vector2.h"

namespace gde {
	namespace input {
		struct WasdDelta : public InputAction {
			Vector2 delta;
		};
	}
}