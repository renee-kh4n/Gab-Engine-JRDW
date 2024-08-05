#pragma once

#include "../ActionImplementation.h"
#include "../Action/MouseRightDrag.h"

namespace gde {
	namespace input {
		class MouseRightDragImplementation : public ActionImplementation<MouseRightDrag> {
		private:
			Vector2 old_mouse_pos;
			// Inherited via ActionImplementation
			virtual void UpdateState(Window* target) override;
		};
	}
}