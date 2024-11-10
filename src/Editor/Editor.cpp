#include "Editor.h"

gbe::Editor::Editor()
{
}

void gbe::Editor::Run(int& argc, char* argv[])
{
    auto editor_app = new gbe::editor::EditorApp();
    wxApp::SetInstance(editor_app);
    wxEntry(argc, (wxChar**)argv);
    wxEntryCleanup();
}
