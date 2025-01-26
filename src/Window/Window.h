/*
Primary file to change if changing window backends. This is the primary interface that connects the engine to the window.

Current backend used: SDL2
*/

#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <queue>

#include <SDL2/SDL.h>

#include "Engine/Input/KeyDefines.h"

#include "../Math/gbe_math.h"
#include "Window/WindowEvents.h"

namespace gbe {

    class Window {
    private:
        SDL_Window* implemented_window;
        SDL_GLContext context;
        Vector2Int mousePos;
        Vector2Int dimentions;

        std::unordered_map<std::string, std::function<void(void*)>> window_callbacks;
        std::unordered_map<unsigned int, bool> keystates;
        
        void* (*procaddressfunc)(const char*);
        bool shouldclose = false;

        std::queue<gbe::window::WindowEventType> event_queue;

        bool centered_cursor = false;;
    public:
        Window(Vector2Int dimentions);

        bool PollEvents(gbe::window::WindowEventType& event_type);

        void UpdateState();
        void SwapBuffers();
        void Terminate();

        bool ShouldClose();

        Vector2Int Get_dimentions();

        void RegisterWindowCallback(const std::string, std::function<void(void*)>);
        void InvokeWindowCallback(const std::string, void*);

        bool GetKeyState(unsigned int keyId);
        Vector2Int GetMousePos();
        void Set_cursorLock(bool);

        void* (*Get_procaddressfunc())(const char*);

    };
}