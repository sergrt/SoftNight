#pragma once

#include <wx/wx.h>

class Hotkey {
public:
    Hotkey(int key, int modifiers, wxChar unicodeKey = WXK_NONE);

    int GetKey() const;
    int GetModifiers() const;
    wxChar GetUnicodeKey() const;

    void SetKey(int key);
    void SetModifiers(int modifiers);
    void SetUnicodeKey(wxChar unicodeKey);

    bool operator==(const Hotkey& other) const;

private:
    int key_{};
    int modifiers_{};
    wxChar unicodeKey_{};
};

Hotkey CreateInvalidHotkey();
