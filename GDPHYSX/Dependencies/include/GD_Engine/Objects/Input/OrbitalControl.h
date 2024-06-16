#pragma once

#include "../Object.h"
#include "InputCustomer.h"
#include "../../Input/Action/MouseRightDrag.h"

namespace gde {
	class OrbitalControl : public InputCustomer<MouseRightDrag> {
		// Inherited via InputCustomer
		virtual void OnInput(MouseRightDrag* value, InputAction::State state) override;
	};
}