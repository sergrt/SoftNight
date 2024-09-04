#include "settings.h"

#include <nlohmann/json.hpp>

#include <fstream>
//#include <stdexcept>

Settings LoadSettings(const std::string& settingsFileName) {
    Settings settings;

    nlohmann::json json;
    {
        std::ifstream stream(settingsFileName);
        json = nlohmann::json::parse(stream);
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
