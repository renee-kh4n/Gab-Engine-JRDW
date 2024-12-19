#pragma once

#include "Engine/Input/ActionImplementation.h"
#include "Engine/Input/Action/MouseScroll.h"
#include "Engine/Input/KeyDefines.h"

#include "Window/gbe_window.h"

namespace gbe {
    namespace input {
        class MouseScrollImplementation : public ActionImplementation<MouseScroll> {
        public:
            // Inherited via ActionImplementation
            void UpdateState(Window* target);
            virtual void ResetState(Window* target);
        };
    }
}