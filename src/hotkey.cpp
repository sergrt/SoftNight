#include "hotkey.h"

Hotkey::Hotkey(int key, int modifiers, wxChar unicodeKey)
    : key_(key)
    , modifiers_(modifiers)
    , unicodeKey_(unicodeKey) {}

int Hotkey::GetKey() const {
    return key_;
}

int Hotkey::GetModifiers() const {
    return modifiers_;
}

wxChar Hotkey::GetUnicodeKey() const {
    return unicodeKey_;
}

void Hotkey::SetKey(int key) {
    key_ = key;
}

void Hotkey::SetModifiers(int modifiers) {
    modifiers_ = modifiers;
}

void Hotkey::SetUnicodeKey(wxChar unicodeKey) {
    unicodeKey_ = unicodeKey;
}

bool Hotkey::operator==(const Hotkey& other) const {
    return key_ == other.key_ && modifiers_ == other.modifiers_ && unicodeKey_ == other.unicodeKey_;
}

Hotkey CreateInvalidHotkey() {
    return Hotkey{0, 0, WXK_NONE};
}
