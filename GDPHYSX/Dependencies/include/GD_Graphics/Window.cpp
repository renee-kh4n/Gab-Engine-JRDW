#include "Window.h"

namespace gde {
    using namespace rendering;

    Window::Window(const char* name, int win_x, int win_y)
    {
        this->win_x = win_x;
        this->win_y = win_y;

        /* Create a windowed mode window and its OpenGL context */
        this->window = glfwCreateWindow(win_x, win_y, name, NULL, NULL);
        if (!this->window)
        {
            glfwTerminate();
        }

        glfwSetWindowUserPointer(this->window, this);

        /* Make the window's context current */
        glfwMakeContextCurrent(this->window);
    }
    GLFWwindow* rendering::Window::Get_window()
    {
        return this->window;
    }
    int rendering::Window::Get_win_x()
    {
        return this->win_x;
    }
    int rendering::Window::Get_win_y()
    {
        return this->win_y;
    }
    void rendering::Window::RegisterWindowCallback(const std::string key, std::function<void(void*)> func)
    {
        this->window_callbacks.insert_or_assign(key, func);
    }
    void rendering::Window::InvokeWindowCallback(const std::string key, void* parameter_object)
    {
        if (this->window_callbacks.find(key) == this->window_callbacks.end()) {
            return;
        }
        else {
            this->window_callbacks[key](parameter_object);
        }
    }
}