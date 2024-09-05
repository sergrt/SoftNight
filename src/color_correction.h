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
    static WORD gamma_array[3][256];

    auto rgb = KtoRgb(colorSettings.temperatureK);

    //m_label->SetLabelText("R: " + std::to_string(rgb.R) + " G: " + std::to_string(rgb.G) + " B: " + std::to_string(rgb.B));

    int gamma_diff = colorSettings.brightness - 256;

    const int r_portion = rgb.R + 128 + gamma_diff;
    const int g_portion = rgb.G + 128 + gamma_diff;
    const int b_portion = rgb.B + 128 + gamma_diff;

    for (int i = 0; i < 256; i++) {
        int arr_r = i * r_portion;
        arr_r = std::clamp(arr_r, 0, 65535);
        int arr_g = i * g_portion;
        arr_g = std::clamp(arr_g, 0, 65535);
        int arr_b = i * b_portion;
        arr_b = std::clamp(arr_b, 0, 65535);

        gamma_array[0][i] = static_cast<WORD>(arr_r);
        gamma_array[1][i] = static_cast<WORD>(arr_g);
        gamma_array[2][i] = static_cast<WORD>(arr_b);
    }

    HDC hdc = GetDC(GetDesktopWindow());
    bool result = SetDeviceGammaRamp(hdc, gamma_array);
    (void)result;
    ReleaseDC(NULL, hdc);
}
/*
inline void Ramp(const ColorSettings& colorSettings) {
    static WORD gamma_array[3][256];
    
    for (int i = 0; i < 256; i++) {
        int arr = i * (colorSettings.brightness + 128);  // GamaRate 128 = Normal
        if (arr > 65535)
            arr = 65535;

        gamma_array[0][i] = gamma_array[1][i] = gamma_array[2][i] = static_cast<WORD>(arr);
    }
    HDC hdc = GetDC(GetDesktopWindow());
    SetDeviceGammaRamp(hdc, gamma_array);

    ReleaseDC(NULL, hdc);
}
*/
