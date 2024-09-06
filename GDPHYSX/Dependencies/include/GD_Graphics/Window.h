#pragma once

#include "util.h"
#include <unordered_map>
#include <functional>

namespace gde {
    namespace rendering {
        class Window {
        private:
            GLFWwindow* window;
            int win_x;
            int win_y;
            
            std::unordered_map<std::string, std::function<void(void*)>> window_callbacks;
        public:
            Window(const char* name, int win_x = 700, int win_y = 700);

            GLFWwindow* Get_window();
            int Get_win_x();
            int Get_win_y();

            void RegisterWindowCallback(const std::string, std::function<void(void*)>);
            void InvokeWindowCallback(const std::string, void*);
        };
    }
}