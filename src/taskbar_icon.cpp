#include "taskbar_icon.h"

#include <wx/menu.h>
#include <wx/dialog.h>

namespace {

enum {
    POPUP_MENU_RESTORE = 10001,
    POPUP_MENU_EXIT,
};

} // namespace

// clang-format off
wxBEGIN_EVENT_TABLE(TaskbarIcon, wxTaskBarIcon)
    EVT_MENU(POPUP_MENU_RESTORE, TaskbarIcon::OnMenuRestore)
    EVT_MENU(POPUP_MENU_EXIT, TaskbarIcon::OnMenuExit)
    EVT_TASKBAR_LEFT_UP(TaskbarIcon::ShowHide)
wxEND_EVENT_TABLE()
// clang-format on

TaskbarIcon::TaskbarIcon(wxDialog* mainWindow)
    : wxTaskBarIcon(wxTBI_DEFAULT_TYPE)
    , mainWindow_{mainWindow} {}

wxMenu* TaskbarIcon::CreatePopupMenu() {
    auto menu = new wxMenu;
    menu->Append(POPUP_MENU_RESTORE, "&Restore");
    menu->AppendSeparator();
    menu->Append(POPUP_MENU_EXIT, "E&xit");
    return menu;
}

void TaskbarIcon::OnMenuRestore(wxCommandEvent& WXUNUSED(event)) {
    RestoreMainWindow();
}

void TaskbarIcon::OnMenuExit(wxCommandEvent& WXUNUSED(event)) {
    mainWindow_->Close(true);
}

void TaskbarIcon::ShowHide(wxTaskBarIconEvent& WXUNUSED(event)) {
    if (!mainWindow_->IsVisible()) {
        RestoreMainWindow();
    } else {
        mainWindow_->Show(false);
    }
}
void TaskbarIcon::RestoreMainWindow() const {
    mainWindow_->Show(true);
    mainWindow_->Restore();
}
