#pragma once


inline const int kDefaultTemperatureK = 6500;
inline const int kDefaultBrightness = 128;

struct ColorSettings {
    int temperatureK{kDefaultTemperatureK};
    int brightness{kDefaultBrightness};
};
