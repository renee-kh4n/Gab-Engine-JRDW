#pragma once

#include "Engine/Input/InputAction.h"

namespace gbe {
	namespace input {
		template<int K>
		struct KeyPress : public InputAction {
			const unsigned int key = K;
		};
	}
}