#include "taskbar.h"

#include <wx/menu.h>
#include <wx/msgdlg.h>

enum {
    PU_RESTORE = 10001,
    PU_NEW_ICON,
    PU_EXIT,
    PU_CHECKMARK,
    PU_SUB1,
    PU_SUB2,
    PU_SUBMAIN };

// clang-format off
wxBEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, MyTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT, MyTaskBarIcon::OnMenuExit)
    //EVT_MENU(PU_NEW_ICON, MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_MENU(PU_CHECKMARK, MyTaskBarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK, MyTaskBarIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_SUB1, MyTaskBarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, MyTaskBarIcon::OnMenuSub)
wxEND_EVENT_TABLE()
// clang-format on

MyTaskBarIcon::MyTaskBarIcon(wxDialog* dialog)
    : wxTaskBarIcon(wxTBI_DEFAULT_TYPE)
    , m_dialog{dialog} {}

void MyTaskBarIcon::OnMenuRestore(wxCommandEvent &) {
    m_dialog->Show(true);
}

void MyTaskBarIcon::OnMenuExit(wxCommandEvent &) {
    m_dialog->Close(true);
}

static bool check = true;

void MyTaskBarIcon::OnMenuCheckmark(wxCommandEvent &) {
    check = !check;
}

void MyTaskBarIcon::OnMenuUICheckmark(wxUpdateUIEvent &event) {
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
void MyTaskBarIcon::OnMenuSub(wxCommandEvent &) {
    wxMessageBox("You clicked on a submenu!");
}

// Overridables
wxMenu *MyTaskBarIcon::CreatePopupMenu() {
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

void MyTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent &) {
    m_dialog->Show(true);
    m_dialog->Restore();
}
