#pragma once

inline const int kMinTemperatureK = 50;
inline const int kMaxTemperatureK = 12000;
inline const int kDefaultTemperatureK = 6500;
inline const int kTemperatureStep = 50;

inline const int kMinBrightness = 0;
inline const int kMaxBrightness = 255; // 256?
inline const int kDefaultBrightness = 128;
inline const int kBrightnessStep = 5;


struct ColorSettings {
    int temperatureK{kDefaultTemperatureK};
    int brightness{kDefaultBrightness};
};
