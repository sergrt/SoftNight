#pragma once
#include "hotkey.h"

#include <wx/textctrl.h>

#include <memory>


class HotkeyInput : public wxTextCtrl
{
public:
    HotkeyInput(wxWindow* parent, wxWindowID id, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0L);

    void OnKeyDown(wxKeyEvent& event);

    void SetHotkey(const Hotkey& hotkey);
    Hotkey GetHotkey() const;
    void ClearHotkey();

private:
    void UpdateText(int key, wxChar unicodeKey, int modifiers);

    Hotkey hotkey_{CreateInvalidHotkey()};

    wxDECLARE_EVENT_TABLE();
};
