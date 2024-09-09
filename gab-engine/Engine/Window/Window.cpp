#include "Window.h"
#include "Window.h"
#include "Window.h"
#include "Window.h"

namespace gde {
    
    gde::rendering::Window::Window()
    {
        implemented_window = SDL_CreateWindow("GabEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);

        for (size_t i = 0; i < gde::Keys::_TOTALKEYS; i++)
        {
            this->keystates.insert_or_assign(i, false);
        }
    }
    void gde::rendering::Window::UpdateState()
    {
        SDL_PumpEvents();
        static const auto keystates = SDL_GetKeyboardState(nullptr);

        for (size_t i = 0; i < length; i++)
        {

        }
    }
    void rendering::Window::SwapBuffers()
    {
        SDL_GL_SwapWindow(this->implemented_window);
    }
    void rendering::Window::Terminate()
    {

    }
    bool rendering::Window::ShouldClose()
    {
        return false;
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
    bool gde::rendering::Window::GetKeyState(unsigned int keyId)
    {
        return this->keystates[keyId];
    }
    Vector2 rendering::Window::GetMousePos()
    {
        SDL_GetMouseState(&this->mousePos.x, &this->mousePos.y);
        return this->mousePos;
    }
}