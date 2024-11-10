#pragma once

// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>
#include "RootWindow.h"

namespace gbe {
    namespace editor {
        class EditorApp : public wxApp
        {
        private:
            RootWindow* rootwindow;
        public:
            bool OnInit() override;
        };
    }
}