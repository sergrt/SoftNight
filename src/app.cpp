#include "app.h"

#include "main_window.h"

wxIMPLEMENT_APP(SoftNight);

bool SoftNight::OnInit() {
    if (!wxApp::OnInit())
        return false;

    auto mainWindow = new MainWindow(nullptr, wxID_ANY, "SoftNight color control");
    (void)mainWindow;
    //mainWindow->Show(true);
    return true;
}
