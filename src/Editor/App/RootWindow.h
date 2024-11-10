#pragma once

#include <wx/wx.h>

namespace gbe {
    namespace editor {
        class RootWindow : public wxFrame
        {
        public:
            RootWindow();

        private:
            void OnHello(wxCommandEvent& event);
            void OnExit(wxCommandEvent& event);
            void OnAbout(wxCommandEvent& event);
        };
    }
}