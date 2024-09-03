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
private:
protected:
    enum {
        DEC_COLOR_TEMP_APPLY = 6000,
        INC_COLOR_TEMP_APPLY,
        DEC_BRIGHTNESS_APPLY,
        INC_BRIGHTNESS_APPLY,
        ENABLE_DISABLE_APPLY,

        DEC_COLOR_TEMP_CLEAR,
        INC_COLOR_TEMP_CLEAR,
        DEC_BRIGHTNESS_CLEAR,
        INC_BRIGHTNESS_CLEAR,
        ENABLE_DISABLE_CLEAR,

        TEMPERATURE_SLIDER,
        BRIGHTNESS_SLIDER,
    };
    wxRadioButton* daySelect;
    wxRadioButton* nightSelect;
    //wxStaticText* m_staticText3;
    wxSlider* temperatureSlider;
    //wxStaticText* m_staticText5;
    //wxStaticText* m_staticText7;
    wxSlider* brightnessSlider;
    //wxStaticText* temperatureDescription;
    //wxStaticText* m_staticText11;
    wxTimePickerCtrl* timePicker;
    wxButton* apply;
    //wxStaticText* m_staticText12;
    HotkeyInput* decColorTemp;
    wxButton* decColorTempApply;
    wxButton* decColorTempClear;
    //wxStaticText* m_staticText13;
    HotkeyInput* incColorTemp;
    wxButton* incColorTempApply;
    wxButton* incColorTempClear;
    //wxStaticText* m_staticText14;
    HotkeyInput* decBrightness;
    wxButton* decBrightnessApply;
    wxButton* decBrightnessClear;
    //wxStaticText* m_staticText15;
    HotkeyInput* incBrightness;
    wxButton* incBrightnessApply;
    wxButton* incBrightnessClear;
    //wxStaticText* m_staticText16;
    HotkeyInput* enableDisable;
    wxButton* enableDisableApply;
    wxButton* enableDisableClear;

public:
    MainWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString,
              const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(454, 489),
              long style = wxCAPTION | wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxSYSTEM_MENU);

    ~MainWindow();




    void DecreaseBrightness();
    void IncreaseBrightness();
    void DefaultBrightness();
    void DecreaseTemperature();
    void IncreaseTemperature();
    void Ramp();
    void Ramp2();
    int cur_brightness_ = 128;
    unsigned short cur_temperature_ = 5000;
    bool last_result_ = true;

    std::unique_ptr<MyTaskBarIcon> m_taskBarIcon;


    void ApplyDecBrightnessHotkey(wxCommandEvent& event);
    void ApplyIncBrightnessHotkey(wxCommandEvent& event);
    void ApplyDecColorTempHotkey(wxCommandEvent& event);
    void ApplyIncColorTempHotkey(wxCommandEvent& event);
    void ApplyEnableDisableHotkey(wxCommandEvent& event);
    void ClearDecBrightnessHotkey(wxCommandEvent& event);
    void ClearIncBrightnessHotkey(wxCommandEvent& event);
    void ClearDecColorTempHotkey(wxCommandEvent& event);
    void ClearIncColorTempHotkey(wxCommandEvent& event);
    void ClearEnableDisableHotkey(wxCommandEvent& event);

    void OnTemperatureSlider(wxCommandEvent& event);
    void OnBrightnessSlider(wxCommandEvent& event);
    void OnHotkey(wxKeyEvent& event);
    void OnCloseWindow(wxCloseEvent& WXUNUSED(event));

    void OnIconize(wxIconizeEvent& event);
    void onResize(wxSizeEvent& event) {
        // layout everything in the dialog
        //temperatureDescription->Wrap(event.GetSize().GetWidth());
        Layout();

        event.Skip();
    }


    void RegisterHotKeys();
    void UpdateHotkeysFields();

    //void onActivate(wxActivateEvent& event) {
    //    UpdateHotkeysFields();
    //}
    void UpdateSliders();
    

private:
    Settings settings_{};
    
    
    
    
    
    
    
    wxDECLARE_EVENT_TABLE();

};

