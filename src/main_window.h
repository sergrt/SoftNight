#pragma once
#include "hotkey_input.h"
#include "settings.h"
#include "taskbar.h"

//#include <wx/artprov.h>
//#include <wx/bitmap.h>
//#include <wx/button.h>
//#include <wx/colour.h>
//#include <wx/dateevt.h>
#include <wx/dialog.h>
//#include <wx/font.h>
//#include <wx/gdicmn.h>
//#include <wx/icon.h>
//#include <wx/image.h>
//#include <wx/intl.h>
#include <wx/radiobut.h>
//#include <wx/settings.h>
//#include <wx/sizer.h>
#include <wx/slider.h>
//#include <wx/statbox.h>
//#include <wx/stattext.h>
//#include <wx/string.h>
//#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/timectrl.h>
//#include <wx/xrc/xmlres.h>

#include <memory>


class MainWindow : public wxDialog {
public:
    MainWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
               long style = wxCAPTION | wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxSYSTEM_MENU);

    ~MainWindow();

private:
    void IncreaseTemperature();
    void DecreaseTemperature();
    void IncreaseBrightness();
    void DecreaseBrightness();
    //void DefaultBrightness();
    void EnableDisable();

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
    void OnHotkey(wxKeyEvent& event);
    void OnCloseWindow(wxCloseEvent& WXUNUSED(event));

    void OnIconize(wxIconizeEvent& event);
    //void onResize(wxSizeEvent& event);

    void RegisterHotKeys();
    void UpdateHotkeysFields();
    void UpdateTimeField();

    // void onActivate(wxActivateEvent& event) {
    //     UpdateHotkeysFields();
    // }
    void UpdateSliders();
    void OnApply(wxCommandEvent& event);
    void SwitchToDay(wxCommandEvent& event);
    void SwitchToNight(wxCommandEvent& event);

    void UpdateColorsOnTimer(wxTimerEvent& event);

    wxRadioButton* daySelect_{};
    wxRadioButton* nightSelect_{};
    wxSlider* temperatureSlider_{};
    wxSlider* brightnessSlider_{};
    wxTimePickerCtrl* timePicker_{};
    HotkeyInput* decTemperature_{};
    HotkeyInput* incTemperature_{};
    HotkeyInput* decBrightness_{};
    HotkeyInput* incBrightness_{};
    HotkeyInput* enableDisable_{};
    wxTimer* colorTimer_{};

    Settings settings_{};
    struct SwitchColorInfo {
        unsigned long long epochTimeToSwitch{};
        ColorSettings switchToColor{};
    } switchColorInfo_;

    void UpdateSwitchColorInfo();
    void StartUpdateColorsTimer();
    void StopUpdateColorsTimer();

    //ColorSettings current_{};

    std::unique_ptr<MyTaskBarIcon> m_taskBarIcon;


    wxDECLARE_EVENT_TABLE();
};
