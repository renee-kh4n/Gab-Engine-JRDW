#pragma once

#include "../Object.h"
#include "InputCustomer.h"
#include "../../Input/Action/WasdDelta.h"

namespace gde {
	class OrbitalControl : public Object, public InputCustomer<WasdDelta> {
	private:
		Vector2 orbital_rotation;
	public:
		// Inherited via InputCustomer
		virtual void OnInput(WasdDelta* value, bool changed) override;
	};
}