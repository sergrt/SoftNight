#include "hotkey_input.h"

#pragma push_macro("DELETE") // Conflicts with windows define
#undef DELETE

const char* GetVirtualKeyCodeName(int keycode) {
    switch (keycode) {
#define WXK_(x)   \
    case WXK_##x: \
        return #x;

        WXK_(BACK)
        WXK_(TAB)
        WXK_(RETURN)
        WXK_(ESCAPE)
        WXK_(SPACE)
        WXK_(DELETE)
        WXK_(START)
        WXK_(LBUTTON)
        WXK_(RBUTTON)
        WXK_(CANCEL)
        WXK_(MBUTTON)
        WXK_(CLEAR)
        // ALT, CTRL, SHIFT are handled as modifiers
        // WXK_(SHIFT)
        // WXK_(ALT)
        // WXK_(CONTROL)
        WXK_(MENU)
        WXK_(PAUSE)
        WXK_(CAPITAL)
        WXK_(END)
        WXK_(HOME)
        WXK_(LEFT)
        WXK_(UP)
        WXK_(RIGHT)
        WXK_(DOWN)
        WXK_(SELECT)
        WXK_(PRINT)
        WXK_(EXECUTE)
        WXK_(SNAPSHOT)
        WXK_(INSERT)
        WXK_(HELP)
        WXK_(NUMPAD0)
        WXK_(NUMPAD1)
        WXK_(NUMPAD2)
        WXK_(NUMPAD3)
        WXK_(NUMPAD4)
        WXK_(NUMPAD5)
        WXK_(NUMPAD6)
        WXK_(NUMPAD7)
        WXK_(NUMPAD8)
        WXK_(NUMPAD9)
        WXK_(MULTIPLY)
        WXK_(ADD)
        WXK_(SEPARATOR)
        WXK_(SUBTRACT)
        WXK_(DECIMAL)
        WXK_(DIVIDE)
        WXK_(F1)
        WXK_(F2)
        WXK_(F3)
        WXK_(F4)
        WXK_(F5)
        WXK_(F6)
        WXK_(F7)
        WXK_(F8)
        WXK_(F9)
        WXK_(F10)
        WXK_(F11)
        WXK_(F12)
        WXK_(F13)
        WXK_(F14)
        WXK_(F15)
        WXK_(F16)
        WXK_(F17)
        WXK_(F18)
        WXK_(F19)
        WXK_(F20)
        WXK_(F21)
        WXK_(F22)
        WXK_(F23)
        WXK_(F24)
        WXK_(NUMLOCK)
        WXK_(SCROLL)
        WXK_(PAGEUP)
        WXK_(PAGEDOWN)
        WXK_(NUMPAD_SPACE)
        WXK_(NUMPAD_TAB)
        WXK_(NUMPAD_ENTER)
        WXK_(NUMPAD_F1)
        WXK_(NUMPAD_F2)
        WXK_(NUMPAD_F3)
        WXK_(NUMPAD_F4)
        WXK_(NUMPAD_HOME)
        WXK_(NUMPAD_LEFT)
        WXK_(NUMPAD_UP)
        WXK_(NUMPAD_RIGHT)
        WXK_(NUMPAD_DOWN)
        WXK_(NUMPAD_PAGEUP)
        WXK_(NUMPAD_PAGEDOWN)
        WXK_(NUMPAD_END)
        WXK_(NUMPAD_BEGIN)
        WXK_(NUMPAD_INSERT)
        WXK_(NUMPAD_DELETE)
        WXK_(NUMPAD_EQUAL)
        WXK_(NUMPAD_MULTIPLY)
        WXK_(NUMPAD_ADD)
        WXK_(NUMPAD_SEPARATOR)
        WXK_(NUMPAD_SUBTRACT)
        WXK_(NUMPAD_DECIMAL)
        WXK_(NUMPAD_DIVIDE)

        WXK_(WINDOWS_LEFT)
        WXK_(WINDOWS_RIGHT)
#ifdef __WXOSX__
        WXK_(RAW_CONTROL)
#endif
#undef WXK_
        default:
            return NULL;
    }
}
#pragma pop_macro("DELETE")

// clang-format off
wxBEGIN_EVENT_TABLE(HotkeyInput, wxTextCtrl)
    EVT_KEY_DOWN(HotkeyInput::OnKeyDown)
wxEND_EVENT_TABLE()
// clang-format on

HotkeyInput::HotkeyInput(wxWindow* parent, wxWindowID id)
    : wxTextCtrl(parent, id) {}

void HotkeyInput::OnKeyDown(wxKeyEvent& event) {
    auto hotkey = Hotkey{event.GetKeyCode(), event.GetModifiers(), event.GetUnicodeKey()};
    SetHotkey(hotkey);
}

Hotkey HotkeyInput::GetHotkey() const {
    return hotkey_;
}

void HotkeyInput::SetHotkey(const Hotkey& hotkey) {
    hotkey_ = hotkey;
    UpdateText(hotkey.GetKey(), hotkey.GetUnicodeKey(), hotkey.GetModifiers());
}

void HotkeyInput::ClearHotkey() {
    SetHotkey(CreateInvalidHotkey());
}

void HotkeyInput::UpdateText(int key, wxChar unicodeKey, int modifiers) {
    std::string text;

    if (modifiers & wxMOD_CONTROL) {
        text += "+ CTRL";
    };
    if (modifiers & wxMOD_ALT) {
        text += "+ ALT";
    };
    if (modifiers & wxMOD_SHIFT) {
        text += "+ SHIFT";
    };
    if (modifiers & wxMOD_WIN) {
        text += "+ WIN";
    };

    if (!text.empty()) {
        text = text.substr(2);
    }

    std::string keyName;
    if (unicodeKey == WXK_NONE || unicodeKey == WXK_DELETE) { // Some problem with Delete
        auto virtualKeyName = GetVirtualKeyCodeName(key);
        if (virtualKeyName) {
            keyName = std::string(virtualKeyName);
        }
    } else {
        if (unicodeKey >= 32) {
            keyName = static_cast<char>(key);
        } else {
            // It's a control character
            // Maybe something should be done here?
        }
    }

    if (!keyName.empty()) {
        if (!text.empty()) {
            text += " + ";
        }
        text += keyName;
    }

    SetLabelText(text);
}
