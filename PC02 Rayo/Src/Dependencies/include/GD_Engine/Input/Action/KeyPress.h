#pragma once

#include "../InputAction.h"

namespace gde {
	namespace input {
		template<int K>
		struct KeyPress : public InputAction {
			const unsigned int key = K;
		};
	}
}