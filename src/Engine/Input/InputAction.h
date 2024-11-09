#pragma once

namespace gbe {
	namespace input {
		struct InputAction {
			enum State {
				START,
				WHILE,
				END
			} state;

			virtual ~InputAction() {};
		};
	}
}