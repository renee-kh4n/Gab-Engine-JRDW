#include "EditorApp.h"

namespace gbe
{
	namespace editor {
        bool EditorApp::OnInit()
        {
            this->rootwindow = new RootWindow();
            this->rootwindow->Show(true);

            return true;
        }
	}
}