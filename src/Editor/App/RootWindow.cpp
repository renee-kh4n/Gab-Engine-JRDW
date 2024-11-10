#include "RootWindow.h"

namespace gbe {
    namespace editor {
        RootWindow::RootWindow() : wxFrame(nullptr, wxID_ANY, "Gab Engine")
        {
            wxMenu* menuFile = new wxMenu;
            menuFile->Append(0, "&Hello...\tCtrl-H","Help string shown in status bar for this menu item");
            menuFile->AppendSeparator();
            menuFile->Append(wxID_EXIT);

            wxMenu* menuHelp = new wxMenu;
            menuHelp->Append(wxID_ABOUT);

            wxMenuBar* menuBar = new wxMenuBar;
            menuBar->Append(menuFile, "&File");
            menuBar->Append(menuHelp, "&Help");

            SetMenuBar(menuBar);

            CreateStatusBar();
            SetStatusText("Gab Engine");

            Bind(wxEVT_MENU, &RootWindow::OnHello, this, 0);
            Bind(wxEVT_MENU, &RootWindow::OnAbout, this, wxID_ABOUT);
            Bind(wxEVT_MENU, &RootWindow::OnExit, this, wxID_EXIT);
        }

        void RootWindow::OnExit(wxCommandEvent& event)
        {
            Close(true);
        }

        void RootWindow::OnAbout(wxCommandEvent& event)
        {
            wxMessageBox("This is a wxWidgets Hello World example",
                "About Hello World", wxOK | wxICON_INFORMATION);
        }

        void RootWindow::OnHello(wxCommandEvent& event)
        {
            wxMessageBox("Hello Log",
                "Hello!", wxOK | wxICON_INFORMATION);
        }
    }
}