#include "app.h"

#include "main_window.h"

wxIMPLEMENT_APP(SoftNight);

bool SoftNight::OnInit() {
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if (!wxApp::OnInit())
        return false;

    // create the main application window
    auto mainWindow = new MainWindow(nullptr, -1, "SoftNight color control");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    //frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.

    //wx::RegisterGlobalHotKey(frame, 1, wxMOD_NONE, 'Q');
    //frame->RegisterHotKey
    //settings_dialog->RegisterHotKey(1, wxMOD_ALT, WXK_PAGEDOWN);
    mainWindow->Show(true);
    return true;
}

