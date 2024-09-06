#include "taskbar_icon.h"

#include <wx/menu.h>
#include <wx/dialog.h>
//#include <wx/msgdlg.h>

enum {
    PU_RESTORE = 10001,
    PU_EXIT,
};

// clang-format off
wxBEGIN_EVENT_TABLE(TaskbarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, TaskbarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT, TaskbarIcon::OnMenuExit)
    EVT_TASKBAR_LEFT_DCLICK(TaskbarIcon::OnLeftButtonDClick)
wxEND_EVENT_TABLE()
// clang-format on

TaskbarIcon::TaskbarIcon(wxDialog* mainWindow)
    : wxTaskBarIcon(wxTBI_DEFAULT_TYPE)
    , mainWindow_{mainWindow} {}

wxMenu* TaskbarIcon::CreatePopupMenu() {
    auto menu = new wxMenu;
    menu->Append(PU_RESTORE, "&Restore main window");
    menu->AppendSeparator();
    menu->Append(PU_EXIT, "E&xit");
    return menu;
}

void TaskbarIcon::OnMenuRestore(wxCommandEvent& WXUNUSED(event)) {
    RestoreMainWindow();
}

void TaskbarIcon::OnMenuExit(wxCommandEvent& WXUNUSED(event)) {
    mainWindow_->Close(true);
}

void TaskbarIcon::OnLeftButtonDClick(wxTaskBarIconEvent& WXUNUSED(event)) {
    RestoreMainWindow();
}

void TaskbarIcon::RestoreMainWindow() const {
    mainWindow_->Show(true);
    mainWindow_->Restore();
}
