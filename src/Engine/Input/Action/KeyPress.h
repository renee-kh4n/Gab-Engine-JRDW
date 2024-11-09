#pragma once

#include "Input/InputAction.h"

namespace gbe {
	namespace input {
		template<int K>
		struct KeyPress : public InputAction {
			const unsigned int key = K;
		};
	}
}