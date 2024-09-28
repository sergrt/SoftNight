#pragma once
#include "settings.h"
#include "taskbar_icon.h"

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/slider.h>
#include <wx/timer.h>
#include <wx/timectrl.h>

#include <memory>

class HotkeyInput;

class MainWindow : public wxDialog {
public:
    MainWindow(wxWindow* parent, wxWindowID id, const wxString& title);

    ~MainWindow();

private:
    void SetupUi();

    void RegisterHotKeys();
    void UnregisterHotKeys();
    void UpdateHotkeysFields();
    void UpdateTimeField();
    void UpdateColorControls();
    void UpdateSwitchColorInfo(ColorSettings* oppositeColorSettings = nullptr);
    void StartUpdateColorsTimer();
    void StopUpdateColorsTimer();
    void ApplyActiveColors();

    // Hotkey handlers
    void IncreaseTemperature();
    void DecreaseTemperature();
    void IncreaseBrightness();
    void DecreaseBrightness();
    void EnableDisable();

    // UI handlers
    void ApplyIncTemperatureHotkey(wxCommandEvent& event);
    void ApplyDecTemperatureHotkey(wxCommandEvent& event);
    void ApplyIncBrightnessHotkey(wxCommandEvent& event);
    void ApplyDecBrightnessHotkey(wxCommandEvent& event);
    void ApplyEnableDisableHotkey(wxCommandEvent& event);

    void ClearIncTemperatureHotkey(wxCommandEvent& event);
    void ClearDecTemperatureHotkey(wxCommandEvent& event);
    void ClearIncBrightnessHotkey(wxCommandEvent& event);
    void ClearDecBrightnessHotkey(wxCommandEvent& event);
    void ClearEnableDisableHotkey(wxCommandEvent& event);

    void OnTemperatureSlider(wxCommandEvent& event);
    void OnBrightnessSlider(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnHotkey(wxKeyEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    void OnIconize(wxIconizeEvent& event);

    void SwitchToDay(wxCommandEvent& event);
    void SwitchToNight(wxCommandEvent& event);
    void UpdateColorsOnTimer(wxTimerEvent& event);

    wxRadioButton* daySelect_{};
    wxRadioButton* nightSelect_{};
    wxSlider* temperatureSlider_{};
    wxSlider* brightnessSlider_{};
    wxTimePickerCtrl* timePicker_{};
    wxButton* applyButton_{};
    wxButton* resetButton_{};
    HotkeyInput* decTemperature_{};
    wxButton* decColorTempApply_{};
    wxButton* decColorTempClear_{};
    HotkeyInput* incTemperature_{};
    wxButton* incColorTempApply_{};
    wxButton* incColorTempClear_{};
    HotkeyInput* decBrightness_{};
    wxButton* decBrightnessApply_{};
    wxButton* decBrightnessClear_{};
    HotkeyInput* incBrightness_{};
    wxButton* incBrightnessApply_{};
    wxButton* incBrightnessClear_{};
    HotkeyInput* enableDisable_{};

    std::unique_ptr<TaskbarIcon> taskbarIcon_{};
    std::unique_ptr<wxTimer> colorTimer_{};

    struct SwitchColorInfo {
        long long epochTimeToSwitch{};
        enum class SwitchTo { Day, Night };
        SwitchTo switchTo{};
    };

    Settings settings_{};
    SwitchColorInfo switchColorInfo_;

    wxDECLARE_EVENT_TABLE();
};
