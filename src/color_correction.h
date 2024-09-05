#pragma once
#include "color_settings.h"

struct RGB {
    int R{};
    int G{};
    int B{};
};

inline RGB KtoRgb(int temperatureK) {
    // Photo-grade results are for temperature in the range 1000K to 40000K. White light = 6500K

    const double temperature = temperatureK / 100.0;

    // RGB components
    double red{};
    double green{};
    double blue{};

    // RED
    if (temperature <= 66.0) {
        red = 255.0;
    } else {
        red = temperature - 60.0;
        red = 329.698727446 * pow(red, -0.1332047592);
        red = std::clamp(red, 0.0, 255.0);
    }

    // GREEN
    if (temperature <= 66.0) {
        green = temperature;
        green = 99.4708025861 * log(green) - 161.1195681661;
    } else {
        green = temperature - 60.0;
        green = 288.1221695283 * pow(green, -0.0755148492);
    }
    green = std::clamp(green, 0.0, 255.0);

    // BLUE
    if (temperature >= 66.0) {
        blue = 255.0;
    } else if (temperature <= 19.0) {
        blue = 0.0;
    } else {
        blue = temperature - 10.0;
        blue = 138.5177312231 * log(blue) - 305.0447927307;
        blue = std::clamp(blue, 0.0, 255.0);
    }

    return {static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue)};
}

inline void Ramp2(const ColorSettings& colorSettings) {
    static WORD gammaArray[3][256];

    // This function is called from color thread, so save some resources by storing last color settings
    static auto lastColorSettings = ColorSettings{0, 0};

    if (lastColorSettings != colorSettings) {
        lastColorSettings = colorSettings;

        auto rgb = KtoRgb(colorSettings.temperatureK);
        int gammaDiff = colorSettings.brightness - 256;

        const int rPortion = rgb.R + 128 + gammaDiff;
        const int gPortion = rgb.G + 128 + gammaDiff;
        const int bPortion = rgb.B + 128 + gammaDiff;

        for (int i = 0; i < 256; i++) {
            int arr_r = i * rPortion;
            arr_r = std::clamp(arr_r, 0, 65535);
            int arr_g = i * gPortion;
            arr_g = std::clamp(arr_g, 0, 65535);
            int arr_b = i * bPortion;
            arr_b = std::clamp(arr_b, 0, 65535);

            gammaArray[0][i] = static_cast<WORD>(arr_r);
            gammaArray[1][i] = static_cast<WORD>(arr_g);
            gammaArray[2][i] = static_cast<WORD>(arr_b);
        }
    }

    if (HDC hdc = GetDC(GetDesktopWindow())) {
        bool result = SetDeviceGammaRamp(hdc, gammaArray);
        (void)result;
        ReleaseDC(NULL, hdc);
    }
}
