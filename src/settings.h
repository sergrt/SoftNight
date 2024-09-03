#pragma once
#include "hotkey.h"

struct Settings {
    Hotkey decTemperature{WXK_DELETE, wxMOD_ALT, WXK_DELETE};
    Hotkey incTemperature{WXK_INSERT, wxMOD_ALT};
    Hotkey decBrightness{WXK_PAGEDOWN, wxMOD_ALT};
    Hotkey incBrightness{WXK_PAGEUP, wxMOD_ALT};
    Hotkey enableDisable{WXK_END, wxMOD_ALT};
};
