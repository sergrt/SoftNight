#pragma once

#include "hotkey.h"

#include <wx/textctrl.h>

class HotkeyInput : public wxTextCtrl
{
public:
    HotkeyInput(wxWindow* parent, wxWindowID id = wxID_ANY);

    void OnKeyDown(wxKeyEvent& event);
    void SetHotkey(const Hotkey& hotkey);
    Hotkey GetHotkey() const;
    void ClearHotkey();

private:
    void UpdateText(int key, wxChar unicodeKey, int modifiers);

    Hotkey hotkey_{CreateInvalidHotkey()};

    wxDECLARE_EVENT_TABLE();
};
