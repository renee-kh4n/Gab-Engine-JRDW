#pragma once

#include "GDutil.h"

struct GDWindow {
    GLFWwindow* window;
    int win_x;
    int win_y;

    GDWindow(int win_x = 700, int win_y = 700);
};