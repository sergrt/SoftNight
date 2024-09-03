#include "main_window.h"

#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/artprov.h>
#include <wx/textctrl.h>
#include <wx/uiaction.h>

enum {
BN_APPLY
};
wxBEGIN_EVENT_TABLE(MainWindow, wxDialog)
    EVT_BUTTON(DEC_COLOR_TEMP_APPLY, MainWindow::ApplyDecColorTempHotkey)
    EVT_BUTTON(INC_COLOR_TEMP_APPLY, MainWindow::ApplyIncColorTempHotkey)
    EVT_BUTTON(DEC_BRIGHTNESS_APPLY, MainWindow::ApplyDecBrightnessHotkey)
    EVT_BUTTON(INC_BRIGHTNESS_APPLY, MainWindow::ApplyIncBrightnessHotkey)
    EVT_BUTTON(ENABLE_DISABLE_APPLY, MainWindow::ApplyEnableDisableHotkey)

    EVT_BUTTON(DEC_COLOR_TEMP_CLEAR, MainWindow::ClearDecColorTempHotkey)
    EVT_BUTTON(INC_COLOR_TEMP_CLEAR, MainWindow::ClearIncColorTempHotkey)
    EVT_BUTTON(DEC_BRIGHTNESS_CLEAR, MainWindow::ClearDecBrightnessHotkey)
    EVT_BUTTON(INC_BRIGHTNESS_CLEAR, MainWindow::ClearIncBrightnessHotkey)
    EVT_BUTTON(ENABLE_DISABLE_CLEAR, MainWindow::ClearEnableDisableHotkey)

    EVT_CLOSE(MainWindow::OnCloseWindow)
    EVT_ICONIZE(MainWindow::OnIconize)
    EVT_HOTKEY(wxID_ANY, MainWindow::OnHotkey)

    EVT_SLIDER(TEMPERATURE_SLIDER, MainWindow::OnTemperatureSlider)
    EVT_SLIDER(BRIGHTNESS_SLIDER, MainWindow::OnBrightnessSlider)
    EVT_SIZE(MainWindow::onResize)
    //EVT_ACTIVATE(MainWindow::onActivate)
wxEND_EVENT_TABLE()


void MainWindow::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {
    Destroy();
}
MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
                     long style)
    : wxDialog(parent, id, title, pos, size, wxRESIZE_BORDER | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxMINIMIZE_BOX)
    , m_taskBarIcon{std::make_unique<MyTaskBarIcon>(this)} {

    wxIcon icon(wxIconLocation(R"(c:\Windows\system32\imageres.dll)", -67));
    if (icon.IsOk()) {
        // wxLogMessage("Loaded icon of size %d*%d", icon.GetWidth(), icon.GetHeight());
        int x;
        (void)x;
    }
    SetIcon(icon);
    
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    

    wxFlexGridSizer* mainFlexSizer= new wxFlexGridSizer(2, 1, 0, 0);
    mainFlexSizer->AddGrowableCol(0);
    mainFlexSizer->AddGrowableRow(0);
    mainFlexSizer->SetFlexibleDirection(wxBOTH);
    mainFlexSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    

    

    
    auto colorControlsBox = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Color controls")), wxVERTICAL);

    // Day-night select
    auto dayNightSizer = new wxBoxSizer(wxHORIZONTAL);
    daySelect = new wxRadioButton(colorControlsBox->GetStaticBox(), wxID_ANY, _("Day"), wxDefaultPosition, wxDefaultSize, 0);
    dayNightSizer->Add(daySelect, 0, wxALL, 5);
    nightSelect = new wxRadioButton(colorControlsBox->GetStaticBox(), wxID_ANY, _("Night"), wxDefaultPosition, wxDefaultSize, 0);
    dayNightSizer->Add(nightSelect, 0, wxALL, 5);

    // Sliders and descriptions
    auto temperatureBrightnessSizer = new wxFlexGridSizer(5, 2, 0, 0);
    temperatureBrightnessSizer->AddGrowableCol(1);
    temperatureBrightnessSizer->AddGrowableRow(1);
    temperatureBrightnessSizer->AddGrowableRow(4);
    temperatureBrightnessSizer->SetFlexibleDirection(wxBOTH);
    temperatureBrightnessSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    auto temperatureText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Temperature, K"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->Add(temperatureText, 0, wxALL, 5);

    temperatureSlider = new wxSlider(colorControlsBox->GetStaticBox(), TEMPERATURE_SLIDER, 3000, 100, 10000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(temperatureSlider, 0, wxALL | wxEXPAND, 5);

    auto temperatureDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Photo-grade range is 1000K - 40000K. White light = 6500K.\nRange can be limited by Windows, see README for details"));
    temperatureBrightnessSizer->AddSpacer((10, 10)); 
    temperatureBrightnessSizer->Add(temperatureDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    // Spacer row
    temperatureBrightnessSizer->AddSpacer((10, 20));
    temperatureBrightnessSizer->AddSpacer((10, 20));

    auto brightnessText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Brightness"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->Add(brightnessText, 0, wxALL, 5);
    

    brightnessSlider = new wxSlider(colorControlsBox->GetStaticBox(), BRIGHTNESS_SLIDER, 0, -128, 128, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(brightnessSlider, 0, wxALL | wxEXPAND, 5);

    auto brightnessDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Use brightness slider to tweak color filtering.\nIt controls red/orange - blue/white balance depending on temperature"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->AddSpacer((10, 10)); 
    temperatureBrightnessSizer->Add(brightnessDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);


    // Time and apply
    wxBoxSizer* timeSizer = new wxBoxSizer(wxHORIZONTAL);

    auto turnOnText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Turn on at"));
    timeSizer->Add(turnOnText, 0, wxALIGN_CENTER | wxALL, 5);

    timePicker = new wxTimePickerCtrl(colorControlsBox->GetStaticBox(), wxID_ANY);
    timeSizer->Add(timePicker, 0, wxALIGN_CENTER | wxALL, 5);
    timeSizer->Add(0, 0, 1, wxEXPAND, 5);
    apply = new wxButton(colorControlsBox->GetStaticBox(), wxID_ANY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0);
    timeSizer->Add(apply, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);



    auto colorControlsSizer = new wxFlexGridSizer(3, 1, 0, 0);
    colorControlsSizer->AddGrowableCol(0);
    colorControlsSizer->AddGrowableRow(2);
    colorControlsSizer->SetFlexibleDirection(wxBOTH);
    colorControlsSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    colorControlsSizer->Add(dayNightSizer, 1, wxBOTTOM | wxEXPAND | wxTOP, 5);
    colorControlsSizer->Add(temperatureBrightnessSizer, 1, wxBOTTOM | wxEXPAND | wxTOP, 5);
    colorControlsSizer->Add(timeSizer, 1, wxBOTTOM | wxEXPAND | wxTOP, 25);

    colorControlsBox->Add(colorControlsSizer, 1, wxEXPAND, 5);
    





    auto hotkeysBox = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Hotkeys")), wxVERTICAL);

    auto decColorTempText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Decrease color temp"));
    decColorTemp = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    decColorTempApply = new wxButton(hotkeysBox->GetStaticBox(), DEC_COLOR_TEMP_APPLY, _("Apply"));
    decColorTempClear = new wxButton(hotkeysBox->GetStaticBox(), DEC_COLOR_TEMP_CLEAR, _("Clear"));

    auto incColorTempText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase color temp"));
    incColorTemp = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    incColorTempApply = new wxButton(hotkeysBox->GetStaticBox(), INC_COLOR_TEMP_APPLY, _("Apply"));
    incColorTempClear = new wxButton(hotkeysBox->GetStaticBox(), INC_COLOR_TEMP_CLEAR, _("Clear"));

    auto decBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Decrease brightness"));
    decBrightness = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    decBrightnessApply = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_APPLY, _("Apply"));
    decBrightnessClear = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_CLEAR, _("Clear"));

    auto incBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase Brightness"));
    incBrightness = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    incBrightnessApply = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_APPLY, _("Apply"));
    incBrightnessClear = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_CLEAR, _("Clear"));

    auto enableDisableText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Enable/Disable"));
    enableDisable = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    enableDisableApply = new wxButton(hotkeysBox->GetStaticBox(), ENABLE_DISABLE_APPLY, _("Apply"));
    enableDisableClear = new wxButton(hotkeysBox->GetStaticBox(), ENABLE_DISABLE_CLEAR, _("Clear"));

    auto hotkeysSizer = new wxFlexGridSizer(6, 4, 0, 0);
    hotkeysSizer->AddGrowableRow(5);
    hotkeysSizer->SetFlexibleDirection(wxBOTH);
    hotkeysSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    hotkeysSizer->Add(decColorTempText, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTemp, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTempApply, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTempClear, 0, wxALL, 5);

    hotkeysSizer->Add(incColorTempText, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTemp, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempApply, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempClear, 0, wxALL, 5);

    hotkeysSizer->Add(decBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightness, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessApply, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessClear, 0, wxALL, 5);

    hotkeysSizer->Add(incBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightness, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessApply, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessClear, 0, wxALL, 5);

    hotkeysSizer->Add(enableDisableText, 0, wxALL, 5);
    hotkeysSizer->Add(enableDisable, 0, wxALL, 5);
    hotkeysSizer->Add(enableDisableApply, 0, wxALL, 5);
    hotkeysSizer->Add(enableDisableClear, 0, wxALL, 5);

    hotkeysBox->Add(hotkeysSizer, 1, wxBOTTOM | wxEXPAND | wxTOP, 5);



    mainFlexSizer->Add(colorControlsBox, 1, wxEXPAND | wxALL, 8);
    mainFlexSizer->Add(hotkeysBox, 1, wxEXPAND | wxALL, 8);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainFlexSizer, 1, wxEXPAND, 5);

    this->SetSizer(mainSizer);
    

    UpdateSliders();
    UpdateHotkeysFields();

    this->SetSize({650, 700});
    this->Layout();
    this->Centre(wxBOTH);

    RegisterHotKeys();

    



    
    
}

MainWindow::~MainWindow() {}

enum Hotkeys {
    DEC_TEMPERATURE = 0,
    INC_TEMPERATURE,
    DEC_BRIGHTNESS,
    INC_BRIGHTNESS,
    ENABLE_DISABLE
};

void MainWindow::RegisterHotKeys() {
    RegisterHotKey(Hotkeys::DEC_TEMPERATURE, settings_.decTemperature.GetModifiers(), settings_.decTemperature.GetKey());
    RegisterHotKey(Hotkeys::INC_TEMPERATURE, settings_.incTemperature.GetModifiers(), settings_.incTemperature.GetKey());
    RegisterHotKey(Hotkeys::DEC_BRIGHTNESS, settings_.decBrightness.GetModifiers(), settings_.decBrightness.GetKey());
    RegisterHotKey(Hotkeys::INC_BRIGHTNESS, settings_.incBrightness.GetModifiers(), settings_.incBrightness.GetKey());
    RegisterHotKey(Hotkeys::ENABLE_DISABLE, settings_.enableDisable.GetModifiers(), settings_.enableDisable.GetKey());
}

void MainWindow::UpdateHotkeysFields() {

    decColorTemp->SetHotkey(settings_.decTemperature);
    incColorTemp->SetHotkey(settings_.incTemperature);
    decBrightness->SetHotkey(settings_.decBrightness);
    incBrightness->SetHotkey(settings_.incBrightness);
    enableDisable->SetHotkey(settings_.enableDisable);


    
    /*
    auto w = this->FindFocus();

    
    auto keyInput = wxUIActionSimulator();

    decColorTemp->SetFocus();
    wxYield();
    decColorTemp->Refresh();
    wxYield();
    keyInput.KeyDown(settings_.decTemperature.key, settings_.decTemperature.modifiers);
    keyInput.KeyUp(settings_.decTemperature.key, settings_.decTemperature.modifiers);
    wxYield();

    
    
    wxYield();
    
    incColorTemp->SetFocus();
    wxYield();
    keyInput.KeyDown(settings_.incTemperature.key, settings_.incTemperature.modifiers);
    keyInput.KeyUp(settings_.incTemperature.key, settings_.incTemperature.modifiers);
    wxYield();

    decBrightness->SetFocus();
    keyInput.KeyDown(settings_.decBrightness.key, settings_.decBrightness.modifiers);
    keyInput.KeyUp(settings_.decBrightness.key, settings_.decBrightness.modifiers);
    wxYield();

    incBrightness->SetFocus();
    keyInput.KeyDown(settings_.incBrightness.key, settings_.incBrightness.modifiers);
    keyInput.KeyUp(settings_.incBrightness.key, settings_.incBrightness.modifiers);
    wxYield();

    wxYield();
    enableDisable->SetFocus();
    keyInput.KeyDown(settings_.enableDisable.key, settings_.enableDisable.modifiers);
    keyInput.KeyUp(settings_.enableDisable.key, settings_.enableDisable.modifiers);

    if (w)
        w->SetFocus();
    else {
        this->SetFocus();
    }
    */
}







void MainWindow::ApplyDecBrightnessHotkey(wxCommandEvent& event) {
    settings_.decBrightness = decBrightness->GetHotkey();
    RegisterHotKey(Hotkeys::DEC_BRIGHTNESS, settings_.decBrightness.GetModifiers(), settings_.decBrightness.GetKey());
}
void MainWindow::ApplyIncBrightnessHotkey(wxCommandEvent& event) {
    settings_.incBrightness = incBrightness->GetHotkey();
    RegisterHotKey(Hotkeys::INC_BRIGHTNESS, settings_.incBrightness.GetModifiers(), settings_.incBrightness.GetKey());
}
void MainWindow::ApplyDecColorTempHotkey(wxCommandEvent& event) {
    settings_.decTemperature = decColorTemp->GetHotkey();
    RegisterHotKey(Hotkeys::DEC_TEMPERATURE, settings_.decTemperature.GetModifiers(), settings_.decTemperature.GetKey());
}
void MainWindow::ApplyIncColorTempHotkey(wxCommandEvent& event) {
    settings_.incTemperature = incColorTemp->GetHotkey();
    RegisterHotKey(Hotkeys::INC_TEMPERATURE, settings_.incTemperature.GetModifiers(), settings_.incTemperature.GetKey());
}
void MainWindow::ApplyEnableDisableHotkey(wxCommandEvent& event) {
    settings_.enableDisable = enableDisable->GetHotkey();
    RegisterHotKey(Hotkeys::ENABLE_DISABLE, settings_.enableDisable.GetModifiers(), settings_.enableDisable.GetKey());

}

void MainWindow::ClearDecBrightnessHotkey(wxCommandEvent& event) {
    decBrightness->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_BRIGHTNESS);
}
void MainWindow::ClearIncBrightnessHotkey(wxCommandEvent& event) {
    incBrightness->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_BRIGHTNESS);
}
void MainWindow::ClearDecColorTempHotkey(wxCommandEvent& event) {
    decColorTemp->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_TEMPERATURE);
}
void MainWindow::ClearIncColorTempHotkey(wxCommandEvent& event) {
    incColorTemp->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_TEMPERATURE);
}
void MainWindow::ClearEnableDisableHotkey(wxCommandEvent& event) {
    enableDisable->ClearHotkey();
    UnregisterHotKey(Hotkeys::ENABLE_DISABLE);
}


void MainWindow::OnHotkey(wxKeyEvent& event) {
    Hotkey hotkey{event.GetKeyCode(), event.GetModifiers(), event.GetUnicodeKey()};
    

    if (hotkey == settings_.decBrightness) {
        DecreaseBrightness();
    } else if (hotkey == settings_.incBrightness) {
        IncreaseBrightness();
    } else if (hotkey == settings_.decTemperature) {
        DecreaseTemperature();
    } else if (hotkey == settings_.incTemperature) {
        IncreaseTemperature();
    } else if (hotkey == settings_.enableDisable) {
        DefaultBrightness();
    }


    /*
    auto p = event.GetKeyCode();
    wxCommandEvent e{};
    // OnAbout(e);

    if (p == WXK_PAGEDOWN) {
        DecreaseBrightness();
    } else if (p == WXK_PAGEUP) {
        IncreaseBrightness();
    } else if (p == WXK_END) {
        DefaultBrightness();
    } else if (p == WXK_INSERT) {
        IncreaseTemperature();
    } else if (p == WXK_DELETE) {
        DecreaseTemperature();
    }
    //if (p == last_event_.GetKeyCode()) {
    //    DecreaseBrightness();
    //}
    //auto w = this->FindFocus();
    //event.SetEventObject(w);
    //event.Skip();
    
    return;
    */
}

void MainWindow::OnIconize(wxIconizeEvent& event) {
    if (event.IsIconized()) {
        Show(false);
        // event.Skip();
    } else {
        // event.Skip();
        // SetFocus();
        // Raise();
        // Show(true);
    }
}

struct RGB {
    int R{};
    int G{};
    int B{};
};

RGB KtoRGB2(int temperature_k) {
    // Best results are achieved for temperature input in Kelvin in the range 1000 K to 40000 K. White light = 6500K

    const double temperature = temperature_k / 100.0;

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



void MainWindow::Ramp2() {
    static WORD gamma_array[3][256];

    auto rgb = KtoRGB2(cur_temperature_);

    //m_label->SetLabelText("R: " + std::to_string(rgb.R) + " G: " + std::to_string(rgb.G) + " B: " + std::to_string(rgb.B));

    int gamma_diff = cur_brightness_ - 256;

    for (int i = 0; i < 256; i++) {
        int arr_r = i * (rgb.R + 128 + gamma_diff);
        arr_r = std::clamp(arr_r, 0, 65535);
        int arr_g = i * (rgb.G + 128 + gamma_diff);
        arr_g = std::clamp(arr_g, 0, 65535);
        int arr_b = i * (rgb.B + 128 + gamma_diff);
        arr_b = std::clamp(arr_b, 0, 65535);

        gamma_array[0][i] = static_cast<WORD>(arr_r);
        gamma_array[1][i] = static_cast<WORD>(arr_g);
        gamma_array[2][i] = static_cast<WORD>(arr_b);
    }

    HDC hdc = GetDC(GetDesktopWindow());
    last_result_ = SetDeviceGammaRamp(hdc, gamma_array);

    ReleaseDC(NULL, hdc);
}

void MainWindow::DecreaseBrightness() {
    cur_brightness_ -= 10;
    if (cur_brightness_ < 0)
        cur_brightness_ = 0;
    Ramp2();

    UpdateSliders();
    //UpdateLabel();
}

void MainWindow::IncreaseBrightness() {
    cur_brightness_ += 10;
    if (cur_brightness_ > 256)
        cur_brightness_ = 256;
    Ramp2();
    UpdateSliders();
    //UpdateLabel();
}

void MainWindow::DefaultBrightness() {
    cur_brightness_ = 128;
    cur_temperature_ = 5000;
    Ramp();
    UpdateSliders();
    //UpdateLabel();
}

void MainWindow::DecreaseTemperature() {
    cur_temperature_ -= 100;
    if (cur_temperature_ < 300)
        cur_temperature_ = 300;
    Ramp2();
    UpdateSliders();
    //UpdateLabel();
}

void MainWindow::IncreaseTemperature() {
    cur_temperature_ += 100;
    if (cur_temperature_ > 28000)
        cur_temperature_ = 28000;
    Ramp2();
    UpdateSliders();
    //UpdateLabel();
}


void MainWindow::Ramp() {
    static WORD gamma_array[3][256];
    
    for (int i = 0; i < 256; i++) {
        int arr = i * (cur_brightness_ + 128);  // GamaRate 128 = Normal
        if (arr > 65535)
            arr = 65535;

        gamma_array[0][i] = gamma_array[1][i] = gamma_array[2][i] = static_cast<WORD>(arr);
    }
    HDC hdc = GetDC(GetDesktopWindow());
    last_result_ = SetDeviceGammaRamp(hdc, gamma_array);

    ReleaseDC(NULL, hdc);
}


void MainWindow::OnTemperatureSlider(wxCommandEvent& event) {
    cur_temperature_ = event.GetInt();
    Ramp2();
    //UpdateLabel();tem
}

void MainWindow::OnBrightnessSlider(wxCommandEvent& event) {
    cur_brightness_ = event.GetInt() + 128;
    Ramp2();
    // UpdateLabel();tem
}

void MainWindow::UpdateSliders() {
    temperatureSlider->SetValue(cur_temperature_);
    brightnessSlider->SetValue(cur_brightness_ - 128);
}

