#pragma once
#include "hotkey_input.h"
#include "settings.h"
#include "taskbar.h"

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/dateevt.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/radiobut.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/timectrl.h>
#include <wx/xrc/xmlres.h>

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
    void DefaultBrightness();

    void Ramp();
    void Ramp2();

    void ApplyIncColorTempHotkey(wxCommandEvent& event);
    void ApplyDecColorTempHotkey(wxCommandEvent& event);
    void ApplyIncBrightnessHotkey(wxCommandEvent& event);
    void ApplyDecBrightnessHotkey(wxCommandEvent& event);
    
    
    
    void ApplyEnableDisableHotkey(wxCommandEvent& event);
    void ClearIncColorTempHotkey(wxCommandEvent& event);
    void ClearDecColorTempHotkey(wxCommandEvent& event);
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

    // void onActivate(wxActivateEvent& event) {
    //     UpdateHotkeysFields();
    // }
    void UpdateSliders();
    void OnApply(wxCommandEvent& event);
    void SwitchToDay(wxCommandEvent& event);
    void SwitchToNight(wxCommandEvent& event);

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

    Settings settings_{};

    int cur_brightness_ = 128;
    unsigned short cur_temperature_ = 5000;
    bool last_result_ = true;

    std::unique_ptr<MyTaskBarIcon> m_taskBarIcon;


    wxDECLARE_EVENT_TABLE();
};
