#pragma once

#include "../Object.h"
#include "Objects/Input/InputCustomer.h"
#include "../../Input/Action/WasdDelta.h"

namespace gde {
	class OrbitalControl : public Object, public InputCustomer<WasdDelta> {
	private:
		Vector2 orbital_rotation;
	public:
		virtual void OnInput(WasdDelta* value, bool changed);
	};
}