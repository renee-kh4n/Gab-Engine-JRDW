#pragma once

#include "Editor/Input/ActionImplementation.h"
#include "Editor/Input/Action/WasdDelta.h"

namespace gde {
	namespace input {
		class WasdDeltaImplementation : public ActionImplementation<WasdDelta> {
		private:
			// Inherited via ActionImplementation
			virtual void UpdateState(Window* target) override;
		};
	}
}