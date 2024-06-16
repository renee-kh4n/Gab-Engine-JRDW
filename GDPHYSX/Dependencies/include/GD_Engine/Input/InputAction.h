#pragma once

namespace gde {
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