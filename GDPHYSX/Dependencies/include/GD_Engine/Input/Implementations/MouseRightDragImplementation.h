#pragma once

#include "../ActionImplementation.h"
#include "../Action/MouseRightDrag.h"

namespace gde {
	namespace input {
		class MouseRightDragImplementation : ActionImplementation<MouseRightDrag> {
			// Inherited via ActionImplementation
			virtual void UpdateState() override;
		};
	}
}