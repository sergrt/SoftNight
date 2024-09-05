#include "settings.h"

#include <nlohmann/json.hpp>

#include <fstream>

Settings LoadSettings(const std::string& settingsFileName) {
    Settings settings;

    nlohmann::json json;
    {
        std::ifstream stream(settingsFileName);
        try {
            json = nlohmann::json::parse(stream);
        } catch (...) {
            return {};
        }
    }

    auto readColors = [](const nlohmann::json& json) {
        ColorSettings colorSettings{};
        if (json.contains("temperatureK") && json.contains("brightness")) {
            colorSettings.temperatureK = json.at("temperatureK");
            colorSettings.brightness = json.at("brightness");
        }
        return colorSettings;
    };

    if (json.contains("day_colors")) {
        settings.dayColors = readColors(json["day_colors"]);
    }
    if (json.contains("night_colors")) {
        settings.nightColors = readColors(json["night_colors"]);
    }
    if (json.contains("current_colors")) {
        settings.activeColors = readColors(json["active_colors"]);
    }

    auto readTime = [](const nlohmann::json& json) {
        if (json.contains("hour") && json.contains("minute") && json.contains("second")) {
            return Time{json.at("hour"), json.at("minute"), json.at("second")};
        }
        return Time{0, 0, 0};
    };

    if (json.contains("swith_to_day")) {
        settings.swithToDay = readTime(json["swith_to_day"]);
    }
    if (json.contains("swith_to_night")) {
        settings.swithToNight = readTime(json["swith_to_night"]);
    }

    auto readHotkey = [](const nlohmann::json& json) {
        if (json.contains("key") && json.contains("modifiers") && json.contains("unicodeKey")) {
            return Hotkey{json.at("key"), json.at("modifiers"), json.at("unicodeKey")};
        }

        return CreateInvalidHotkey();
    };
    if (json.contains("inc_temperature")) {
        settings.incTemperature = readHotkey(json["inc_temperature"]);
    }
    if (json.contains("dec_temperature")) {
        settings.decTemperature = readHotkey(json["dec_temperature"]);
    }
    if (json.contains("inc_brightness")) {
        settings.incBrightness = readHotkey(json["inc_brightness"]);
    }
    if (json.contains("dec_brightness")) {
        settings.decBrightness = readHotkey(json["dec_brightness"]);
    }
    if (json.contains("enable_disable")) {
        settings.enableDisable = readHotkey(json["enable_disable"]);
    }

    return settings;
}

void SaveSettings(const Settings& settings, const std::string& settingsFileName) {
    nlohmann::json json;
    
    auto writeColors = [](const ColorSettings& colorSettings, nlohmann::json& json) {
        json["temperatureK"] = colorSettings.temperatureK;
        json["brightness"] = colorSettings.brightness;
    };
    writeColors(settings.dayColors, json["day_colors"]);
    writeColors(settings.nightColors, json["night_colors"]);
    writeColors(settings.isEnabled ? settings.activeColors : settings.backupColors, json["active_colors"]);

    auto writeTime = [](const Time& time, nlohmann::json& json) {
        json["hour"] = time.hour;
        json["minute"] = time.minute;
        json["second"] = time.second;
    };
    writeTime(settings.swithToDay, json["swith_to_day"]);
    writeTime(settings.swithToNight, json["swith_to_night"]);

    auto writeHotkey = [](const Hotkey& hotkey, nlohmann::json& json) {
        json["key"] = hotkey.GetKey();
        json["modifiers"] = hotkey.GetModifiers();
        json["unicodeKey"] = hotkey.GetUnicodeKey();
    };
    writeHotkey(settings.incTemperature, json["inc_temperature"]);
    writeHotkey(settings.decTemperature, json["dec_temperature"]);
    writeHotkey(settings.incBrightness, json["inc_brightness"]);
    writeHotkey(settings.decBrightness, json["dec_brightness"]);
    writeHotkey(settings.enableDisable, json["enable_disable"]);

    std::ofstream stream(settingsFileName);
    stream << json.dump(4);
    stream.close();
};
