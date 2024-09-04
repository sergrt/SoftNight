#pragma once
#include "color_settings.h"
#include "hotkey.h"

#include <chrono>

struct Settings {
    ColorSettings dayColors{};
    ColorSettings nightColors{};

    std::chrono::time_point<std::chrono::steady_clock> lastUpdate{};

    Hotkey incTemperature{WXK_INSERT, wxMOD_ALT};
    Hotkey decTemperature{WXK_DELETE, wxMOD_ALT, WXK_DELETE};
    Hotkey incBrightness{WXK_PAGEUP, wxMOD_ALT};
    Hotkey decBrightness{WXK_PAGEDOWN, wxMOD_ALT};
    Hotkey enableDisable{WXK_END, wxMOD_ALT};
};

Settings LoadSettings(const std::string& settingsFileName);
void SaveSettings(const Settings& settings, const std::string& settingsFileName);
