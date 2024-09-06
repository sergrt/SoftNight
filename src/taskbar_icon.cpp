#include "taskbar_icon.h"

#include <wx/menu.h>
#include <wx/msgdlg.h>

enum {
    PU_RESTORE = 10001,
    PU_NEW_ICON,
    PU_EXIT,
    PU_CHECKMARK,
    PU_SUB1,
    PU_SUB2,
    PU_SUBMAIN
};

// clang-format off
wxBEGIN_EVENT_TABLE(TaskbarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, TaskbarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT, TaskbarIcon::OnMenuExit)
    //EVT_MENU(PU_NEW_ICON, MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_MENU(PU_CHECKMARK, TaskbarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK, TaskbarIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK(TaskbarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_SUB1, TaskbarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, TaskbarIcon::OnMenuSub)
wxEND_EVENT_TABLE()
// clang-format on

TaskbarIcon::TaskbarIcon(wxDialog* mainWindow)
    : wxTaskBarIcon(wxTBI_DEFAULT_TYPE)
    , mainWindow_{mainWindow} {}

void TaskbarIcon::OnMenuRestore(wxCommandEvent &) {
    mainWindow_->Show(true);
}

void TaskbarIcon::OnMenuExit(wxCommandEvent &) {
    mainWindow_->Close(true);
}

static bool check = true;

void TaskbarIcon::OnMenuCheckmark(wxCommandEvent &) {
    check = !check;
}

void TaskbarIcon::OnMenuUICheckmark(wxUpdateUIEvent &event) {
    event.Check(check);
}
/*
void MyTaskBarIcon::OnMenuSetNewIcon(wxCommandEvent &) {
    wxIcon icon(smile_xpm);

    if (!SetIcon(wxBitmapBundle::FromBitmaps(wxBitmap(smile_xpm), wxBitmap(smile2_xpm)),
                 "wxTaskBarIcon Sample - a different icon"))
        wxMessageBox("Could not set new icon.");
}
*/
void TaskbarIcon::OnMenuSub(wxCommandEvent &) {
    wxMessageBox("You clicked on a submenu!");
}

// Overridables
wxMenu* TaskbarIcon::CreatePopupMenu() {
    wxMenu *menu = new wxMenu;
    menu->Append(PU_RESTORE, "&Restore main window");
    menu->AppendSeparator();
    menu->Append(PU_NEW_ICON, "&Set New Icon");
    menu->AppendSeparator();
    menu->AppendCheckItem(PU_CHECKMARK, "Test &check mark");
    menu->AppendSeparator();
    wxMenu *submenu = new wxMenu;
    submenu->Append(PU_SUB1, "One submenu");
    submenu->AppendSeparator();
    submenu->Append(PU_SUB2, "Another submenu");
    menu->Append(PU_SUBMAIN, "Submenu", submenu);
    /* OSX has built-in quit menu for the dock menu, but not for the status item */
    menu->AppendSeparator();
    menu->Append(PU_EXIT, "E&xit");

    return menu;
}

void TaskbarIcon::OnLeftButtonDClick(wxTaskBarIconEvent &) {
    mainWindow_->Show(true);
    mainWindow_->Restore();
}
