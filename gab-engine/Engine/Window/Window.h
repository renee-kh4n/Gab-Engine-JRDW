/*
Primary file to change if changing window backends. This is the primary interface that connects the engine to the window.

Current backend used: SDL2
*/

#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <SDL.h>
#include "../Input/KeyDefines.h"

#include "../Datatypes/Vector2.h"

namespace gde {
    namespace rendering {
        class Window{
        private:
            SDL_Window* implemented_window;

            Vector2 mousePos;
            
            std::unordered_map<std::string, std::function<void(void*)>> window_callbacks;

            std::unordered_map<unsigned int, int> keystates;
        public:
            Window();

            void UpdateState();
            void SwapBuffers();
            void Terminate();

            bool ShouldClose();

            void SetContextToThis();
            void* GetNativeHandle();

            int Get_win_x();
            int Get_win_y();

            void RegisterWindowCallback(const std::string, std::function<void(void*)>);
            void InvokeWindowCallback(const std::string, void*);

            bool GetKeyState(unsigned int keyId);
            Vector2 GetMousePos();
        };
    }
}