#pragma once

#include <wx/dialog.h>
#include <wx/taskbar.h>

class TaskbarIcon : public wxTaskBarIcon {
public:
    TaskbarIcon(wxDialog *mainWindow);

    void OnLeftButtonDClick(wxTaskBarIconEvent &);
    void OnMenuRestore(wxCommandEvent &);
    void OnMenuExit(wxCommandEvent &);
    void OnMenuSetNewIcon(wxCommandEvent &);
    void OnMenuCheckmark(wxCommandEvent &);
    void OnMenuUICheckmark(wxUpdateUIEvent &);
    void OnMenuSub(wxCommandEvent &);
    virtual wxMenu *CreatePopupMenu() wxOVERRIDE;

    wxDialog* mainWindow_{};

    wxDECLARE_EVENT_TABLE();
};
