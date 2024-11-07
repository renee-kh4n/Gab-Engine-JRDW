#include "Window.h"

namespace gde {
    
    gde::Window::Window()
    {
        implemented_window = SDL_CreateWindow("GabEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);

        for (size_t i = 0; i < gde::Keys::_TOTALKEYS; i++)
        {
            this->keystates.insert_or_assign(i, false);
        }
    }
    void gde::Window::UpdateState()
    {
        SDL_PumpEvents();
        static const auto keystates = SDL_GetKeyboardState(nullptr);

        for (size_t i = 0; i < Keys::_TOTALKEYS; i++)
        {

        }
    }
    void Window::SwapBuffers()
    {
        SDL_GL_SwapWindow(this->implemented_window);
    }
    void Window::Terminate()
    {

    }
    bool Window::ShouldClose()
    {
        return false;
    }
    void Window::SetContextToThis()
    {
        
    }
    void* Window::GetNativeHandle()
    {
        return nullptr;
    }
    int Window::Get_win_x()
    {
        return 800;
    }
    int Window::Get_win_y()
    {
        return 800;
    }
    void Window::RegisterWindowCallback(const std::string key, std::function<void(void*)> func)
    {
        this->window_callbacks.insert_or_assign(key, func);
    }
    void Window::InvokeWindowCallback(const std::string key, void* parameter_object)
    {
        if (this->window_callbacks.find(key) == this->window_callbacks.end()) {
            return;
        }
        else {
            this->window_callbacks[key](parameter_object);
        }
    }
    bool gde::Window::GetKeyState(unsigned int keyId)
    {
        return this->keystates[keyId];
    }
    Vector2Int Window::GetMousePos()
    {
        SDL_GetMouseState(&this->mousePos.x, &this->mousePos.y);
        return this->mousePos;
    }
}