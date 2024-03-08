#include "GDWindow.h"

GDWindow::GDWindow()
{
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(win_x, win_y, "Sword summoner", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
}
