#include "app.h"

#include "main_window.h"

wxIMPLEMENT_APP(SoftNight);

bool SoftNight::OnInit() {
    if (!wxApp::OnInit())
        return false;

    wxImage::AddHandler(new wxICOHandler);

    auto mainWindow = new MainWindow(nullptr, wxID_ANY, "SoftNight color control");
    // mainWindow->Show(true);
    (void)mainWindow;

    return true;
}
