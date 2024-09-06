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
            
            Vector2 late_delta;
            Vector2 ontime_delta;
        public:
            // Inherited via ActionImplementation
            void UpdateState(Window* target);
        };
    }
}