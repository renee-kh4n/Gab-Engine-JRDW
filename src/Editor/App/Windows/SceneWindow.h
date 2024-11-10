#pragma once

#include <wx/wx.h>
#include "../RootWindow.h"

namespace gbe {
    namespace editor {
        class SceneWindow : public wxFrame
        {
        public:
            SceneWindow(RootWindow* parent);
        };
    }
}