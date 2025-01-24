#pragma once


#include "Engine/Input/ActionImplementation.h"
#include "Engine/Input/Action/WasdDelta.h"

#include "Window/gbe_window.h"

namespace gbe {
	namespace input {
		class WasdDeltaImplementation : public ActionImplementation<WasdDelta> {
		private:
			void UpdateState(Window* target) override;
			virtual void ResetState(Window* target) override;
		};
	}
}