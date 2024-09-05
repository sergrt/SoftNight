#pragma once
#include "color_settings.h"
#include "hotkey.h"

#include <chrono>

struct Time {
    int hour{};
    int minute{};
    int second{};
};

struct Settings {
    ColorSettings dayColors{};
    ColorSettings nightColors{};
    ColorSettings activeColors{};  // active-backup is used for enabled-disabled mode
    ColorSettings backupColors{};

    Time swithToDay{};
    Time swithToNight{};

    Hotkey incTemperature{WXK_INSERT, wxMOD_ALT};
    Hotkey decTemperature{WXK_DELETE, wxMOD_ALT, WXK_DELETE};
    Hotkey incBrightness{WXK_PAGEUP, wxMOD_ALT};
    Hotkey decBrightness{WXK_PAGEDOWN, wxMOD_ALT};
    Hotkey enableDisable{WXK_END, wxMOD_ALT};

    bool isEnabled{true};
};

Settings LoadSettings(const std::string& settingsFileName);
void SaveSettings(const Settings& settings, const std::string& settingsFileName);
