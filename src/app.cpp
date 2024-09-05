#include "app.h"

#include "main_window.h"

wxIMPLEMENT_APP(SoftNight);

bool SoftNight::OnInit() {
    if (!wxApp::OnInit())
        return false;

    auto mainWindow = new MainWindow(nullptr, -1, "SoftNight color control");

    mainWindow->Show(true);
    return true;
}
