#pragma once
//#include "settings_dialog.h"
#include "hotkey.h"

#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include <memory>

class MyDialog1;


class HotkeyInput : public wxTextCtrl
{
public:
    HotkeyInput(wxWindow* parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0L);
    void OnKeyDown(wxKeyEvent& event);

    MyDialog1* m_dialog{};
    
    void SetHotkey(const Hotkey& hotkey);
    Hotkey GetHotkey() const;
    void ClearHotkey();

private:
    void UpdateText(int key, wxChar unicodeKey, int modifiers);

    Hotkey hotkey_ = CreateInvalidHotkey();

    wxDECLARE_EVENT_TABLE();
};
