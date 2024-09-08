#include "Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace gde {
    using namespace rendering;

    Window::Window(const char* name, int win_x, int win_y)
    {
        this->win_x = win_x;
        this->win_y = win_y;

        /* Create a windowed mode window and its OpenGL context */
        this->glfw_window = glfwCreateWindow(win_x, win_y, name, NULL, NULL);
        if (!this->glfw_window)
        {
            glfwTerminate();
        }

        glfwSetWindowUserPointer(this->glfw_window, this);
    }
    void rendering::Window::SetContextToThis()
    {
        glfwMakeContextCurrent(this->glfw_window);
    }
    void* rendering::Window::GetNativeHandle()
    {
        return glfwGetWin32Window(this->glfw_window);
    }
    GLFWwindow* rendering::Window::Get_window()
    {
        return this->glfw_window;
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