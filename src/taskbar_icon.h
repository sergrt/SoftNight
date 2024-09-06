#pragma once

#include <wx/taskbar.h>

class wxDialog;

class TaskbarIcon : public wxTaskBarIcon {
public:
    TaskbarIcon(wxDialog* mainWindow);

    virtual wxMenu* CreatePopupMenu() wxOVERRIDE;

    void ShowHide(wxTaskBarIconEvent& event);
    void OnMenuRestore(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);

private:
    void RestoreMainWindow() const;

    wxDialog* mainWindow_{};

    wxDECLARE_EVENT_TABLE();
};
