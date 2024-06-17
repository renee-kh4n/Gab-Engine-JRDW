#pragma once

#include "util.h"

namespace gde {
    struct Window {
        GLFWwindow* window;
        int win_x;
        int win_y;

        Window(const char* name, int win_x = 700, int win_y = 700);
    };
}