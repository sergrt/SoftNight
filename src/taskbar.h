#pragma once

#include <wx/dialog.h>
#include <wx/taskbar.h>

class MyTaskBarIcon : public wxTaskBarIcon {
public:
    MyTaskBarIcon(wxDialog *dialog);

    void OnLeftButtonDClick(wxTaskBarIconEvent &);
    void OnMenuRestore(wxCommandEvent &);
    void OnMenuExit(wxCommandEvent &);
    void OnMenuSetNewIcon(wxCommandEvent &);
    void OnMenuCheckmark(wxCommandEvent &);
    void OnMenuUICheckmark(wxUpdateUIEvent &);
    void OnMenuSub(wxCommandEvent &);
    virtual wxMenu *CreatePopupMenu() wxOVERRIDE;

    wxDECLARE_EVENT_TABLE();

    wxDialog *m_dialog{};
};
