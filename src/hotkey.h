#pragma once

#include <wx/wx.h>


class Hotkey {
public:
    Hotkey(int key, int modifiers, wxChar unicodeKey = WXK_NONE)
        : key_(key)
        , modifiers_(modifiers)
        , unicodeKey_(unicodeKey) {}

    // Getters
    int GetKey() const {
        return key_;
    }
    int GetModifiers() const {
        return modifiers_;
    }
    const wxChar GetUnicodeKey() const {
        return unicodeKey_;
    }

    // Setters
    void setKey(int key) {
        key_ = key;
    }
    void setModifiers(int modifiers) {
        modifiers_ = modifiers;
    }
    void setUnicodeKey(wxChar unicodeKey) {
        unicodeKey_ = unicodeKey;
    }

    bool operator==(const Hotkey& other) const {
        return key_ == other.key_ && modifiers_ == other.modifiers_ && unicodeKey_ == other.unicodeKey_;
    }

private:
    int key_{};
    int modifiers_{};
    wxChar unicodeKey_{};
};

/*

class Hotkey {


    int modifiers{};
    int key{};
    wxChar unicodeKey{WXK_NONE};

    bool operator==(const Hotkey& other) const {
        return modifiers == other.modifiers && key == other.key && unicodeKey == other.unicodeKey;
    }
};
*/
inline Hotkey CreateInvalidHotkey() {
    return Hotkey{0, 0, WXK_NONE};
}
