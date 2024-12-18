/*
Primary file to change if changing window backends. This is the primary interface that connects the engine to the window.

Current backend used: SDL2
*/

#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <SDL2/SDL.h>

#include "Engine/Input/KeyDefines.h"

#include "../Datatypes/Vector2Int.h"

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
    public:
        Window(Vector2Int dimentions);

        void UpdateState();
        void SwapBuffers();
        void Terminate();

        bool ShouldClose();

        Vector2Int Get_dimentions();

        void RegisterWindowCallback(const std::string, std::function<void(void*)>);
        void InvokeWindowCallback(const std::string, void*);

        bool GetKeyState(unsigned int keyId);
        Vector2Int GetMousePos();

        void* (*Get_procaddressfunc())(const char*);
    };
}