#pragma once

#include "../ActionImplementation.h"
#include "../Action/MouseScroll.h"

#define GDE_CALLBACKID_MOUSESCROLL "GDE_CALLBACKID_MOUSESCROLL"

namespace gde {
    namespace input {
        class MouseScrollImplementation : public ActionImplementation<MouseScroll> {
        private:
            bool initialized_callback;
            Window* window_cache;
        public:
            // Inherited via ActionImplementation
            void UpdateState(Window* target);
            virtual void ResetState(Window* target);
        };
    }
}