#pragma once

#include "Engine/Objects/Input/InputCustomer.h"
#include "Engine/Input/Action/WasdDelta.h"

namespace gbe {
	class OrbitalControl : public Object, public InputCustomer<WasdDelta> {
	private:
		Vector2 orbital_rotation;
	public:
		virtual void OnInput(WasdDelta* value, bool changed);
	};
}