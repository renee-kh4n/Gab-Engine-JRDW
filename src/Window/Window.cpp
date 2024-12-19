#include "Window.h"

namespace gbe {
    
    gbe::Window::Window(Vector2Int dimentions)
    {
        this->dimentions = dimentions; 

        // Initialize SDL 
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw "Couldn't initialize SDL";
        atexit(SDL_Quit);
        SDL_GL_LoadLibrary(NULL);

        // Request an OpenGL 4.5 context (should be core)
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        // Also request a depth buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        implemented_window = SDL_CreateWindow("GabEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimentions.x, dimentions.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

        this->context = SDL_GL_CreateContext(implemented_window);
        if (this->context == nullptr)
            throw "Failed to create OpenGL context";

        this->procaddressfunc = SDL_GL_GetProcAddress;

        auto thing = this->procaddressfunc("glGetString");

        // Use v-sync
        SDL_GL_SetSwapInterval(0);

        for (size_t i = 0; i < gbe::Keys::_TOTALKEYS; i++)
        {
            this->keystates.insert_or_assign(i, false);
        }
    }

    bool gbe::Window::PollEvents(gbe::window::WindowEventType& event_type) {
        if (this->event_queue.empty())
            return false;

        event_type = this->event_queue.front();
        this->event_queue.pop();
        return true;
    }

    void gbe::Window::UpdateState()
    {
        //States to reset
        this->keystates[gbe::Keys::MOUSE_SCROLL_UP] = false;
        this->keystates[gbe::Keys::MOUSE_SCROLL_DOWN] = false;

        //Handle events on queue
        SDL_Event sdlevent;
        while (SDL_PollEvent(&sdlevent) != 0)
        {
            //User requests quit
            if (sdlevent.type == SDL_QUIT)
            {
                shouldclose = true;
            }
            if (sdlevent.type == SDL_WINDOWEVENT) {
                if (sdlevent.window.event == SDL_WINDOWEVENT_RESIZED) {
                    this->dimentions = Vector2Int(sdlevent.window.data1, sdlevent.window.data2);
                    this->event_queue.push(window::WindowEventType::RESIZE);
                }
            }

            if (sdlevent.type == SDL_MOUSEBUTTONDOWN) {
                if (sdlevent.button.button == SDL_BUTTON_LEFT)
                    this->keystates[gbe::Keys::MOUSE_LEFT] = true;
                if (sdlevent.button.button == SDL_BUTTON_RIGHT)
                    this->keystates[gbe::Keys::MOUSE_RIGHT] = true;
                if (sdlevent.button.button == SDL_BUTTON_MIDDLE)
                    this->keystates[gbe::Keys::MOUSE_MIDDLE] = true;
            }
            if (sdlevent.type == SDL_MOUSEBUTTONUP) {
                if (sdlevent.button.button == SDL_BUTTON_LEFT)
                    this->keystates[gbe::Keys::MOUSE_LEFT] = false;
                if (sdlevent.button.button == SDL_BUTTON_RIGHT)
                    this->keystates[gbe::Keys::MOUSE_RIGHT] = false;
                if (sdlevent.button.button == SDL_BUTTON_MIDDLE)
                    this->keystates[gbe::Keys::MOUSE_MIDDLE] = false;
            }
            if (sdlevent.type == SDL_MOUSEWHEEL) {
                if (sdlevent.wheel.y > 0)
                    this->keystates[gbe::Keys::MOUSE_SCROLL_UP] = true;
                if(sdlevent.wheel.y < 0)
                    this->keystates[gbe::Keys::MOUSE_SCROLL_DOWN] = true;
            }
        }

        auto sdl_keyboardstates = SDL_GetKeyboardState(nullptr);
        auto sdl_mousestates = SDL_GetMouseState(nullptr, nullptr);
    }
    void Window::SwapBuffers()
    {
        SDL_GL_SwapWindow(this->implemented_window);
    }
    void Window::Terminate()
    {
        SDL_Quit();
    }
    bool Window::ShouldClose()
    {
        return this->shouldclose;
    }
    Vector2Int Window::Get_dimentions()
    {
        return this->dimentions;
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
    bool gbe::Window::GetKeyState(unsigned int keyId)
    {
        return this->keystates[keyId];
    }
    Vector2Int Window::GetMousePos()
    {
        SDL_GetMouseState(&this->mousePos.x, &this->mousePos.y);
        return this->mousePos;
    }
    void* (*Window::Get_procaddressfunc())(const char*)
    {
        return this->procaddressfunc;
    }
}