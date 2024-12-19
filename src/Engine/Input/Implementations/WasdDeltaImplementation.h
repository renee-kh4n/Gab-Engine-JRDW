#pragma once


#include "Engine/Input/ActionImplementation.h"
#include "Engine/Input/Action/WasdDelta.h"

#include "Window/gbe_window.h"

namespace gbe {
	namespace input {
		class WasdDeltaImplementation : public ActionImplementation<WasdDelta> {
		private:
			// Inherited via ActionImplementation
			virtual void UpdateState(Window* target) override;
		};
	}
}