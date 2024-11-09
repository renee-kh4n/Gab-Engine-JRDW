#include "Editor.h"

gbe::Editor::Editor()
{
}

void gbe::Editor::Run()
{
    // Create the main frame window
    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "wxWidgets Types Demo", wxPoint(50, 50), wxSize(450, 340));

    wxFileDialog fileDlg(frame, _("Choose the PNG file"), wxEmptyString, wxEmptyString, _("PNG file|*.png|All files|*.*"));
    if (fileDlg.ShowModal() == wxID_OK) {
        wxString path = fileDlg.GetPath();
        //use this path in your app
    }
}
