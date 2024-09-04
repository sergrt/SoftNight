#include "main_window.h"

#include "color_correction.h"
//#include <wx/button.h>
//#include <wx/msgdlg.h>
//#include <wx/sizer.h>
//#include <wx/stattext.h>
//#include <wx/artprov.h>
//#include <wx/textctrl.h>
//#include <wx/uiaction.h>

namespace {

enum {
    START_VALUE = 6000,

    INC_TEMPERATURE_APPLY,
    DEC_TEMPERATURE_APPLY,
    INC_BRIGHTNESS_APPLY,
    DEC_BRIGHTNESS_APPLY,
    ENABLE_DISABLE_APPLY,

    INC_TEMPERATURE_CLEAR,
    DEC_TEMPERATURE_CLEAR,
    INC_BRIGHTNESS_CLEAR,
    DEC_BRIGHTNESS_CLEAR,
    ENABLE_DISABLE_CLEAR,

    BN_APPLY,
    RB_DAY,
    RB_NIGHT,

    TEMPERATURE_SLIDER,
    BRIGHTNESS_SLIDER,
};

} // namespace

// clang-format off
wxBEGIN_EVENT_TABLE(MainWindow, wxDialog)
    EVT_BUTTON(INC_TEMPERATURE_APPLY, MainWindow::ApplyIncTemperatureHotkey)
    EVT_BUTTON(DEC_TEMPERATURE_APPLY, MainWindow::ApplyDecTemperatureHotkey)
    EVT_BUTTON(INC_BRIGHTNESS_APPLY, MainWindow::ApplyIncBrightnessHotkey)
    EVT_BUTTON(DEC_BRIGHTNESS_APPLY, MainWindow::ApplyDecBrightnessHotkey)
    EVT_BUTTON(ENABLE_DISABLE_APPLY, MainWindow::ApplyEnableDisableHotkey)

    EVT_BUTTON(INC_TEMPERATURE_CLEAR, MainWindow::ClearIncTemperatureHotkey)
    EVT_BUTTON(DEC_TEMPERATURE_CLEAR, MainWindow::ClearDecTemperatureHotkey)
    EVT_BUTTON(INC_BRIGHTNESS_CLEAR, MainWindow::ClearIncBrightnessHotkey)
    EVT_BUTTON(DEC_BRIGHTNESS_CLEAR, MainWindow::ClearDecBrightnessHotkey)
    EVT_BUTTON(ENABLE_DISABLE_CLEAR, MainWindow::ClearEnableDisableHotkey)

    EVT_BUTTON(BN_APPLY, MainWindow::OnApply)
    EVT_RADIOBUTTON(RB_DAY, MainWindow::SwitchToDay)
    EVT_RADIOBUTTON(RB_NIGHT, MainWindow::SwitchToNight)

    EVT_SLIDER(TEMPERATURE_SLIDER, MainWindow::OnTemperatureSlider)
    EVT_SLIDER(BRIGHTNESS_SLIDER, MainWindow::OnBrightnessSlider)

    EVT_CLOSE(MainWindow::OnCloseWindow)
    EVT_ICONIZE(MainWindow::OnIconize)
    //EVT_SIZE(MainWindow::onResize)
    EVT_HOTKEY(wxID_ANY, MainWindow::OnHotkey)
wxEND_EVENT_TABLE()
// clang-format on

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
    daySelect_ = new wxRadioButton(colorControlsBox->GetStaticBox(), RB_DAY, _("Day"));
    dayNightSizer->Add(daySelect_, 0, wxALL, 5);
    nightSelect_ = new wxRadioButton(colorControlsBox->GetStaticBox(), RB_NIGHT, _("Night"));
    dayNightSizer->Add(nightSelect_, 0, wxALL, 5);

    // Sliders and descriptions
    auto temperatureBrightnessSizer = new wxFlexGridSizer(5, 2, 0, 0);
    temperatureBrightnessSizer->AddGrowableCol(1);
    temperatureBrightnessSizer->AddGrowableRow(1);
    temperatureBrightnessSizer->AddGrowableRow(4);
    temperatureBrightnessSizer->SetFlexibleDirection(wxBOTH);
    temperatureBrightnessSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    auto temperatureText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Temperature, K"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->Add(temperatureText, 0, wxALL, 5);

    temperatureSlider_ = new wxSlider(colorControlsBox->GetStaticBox(), TEMPERATURE_SLIDER,
        kDefaultTemperatureK, kMinTemperatureK, kMaxTemperatureK, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(temperatureSlider_, 0, wxALL | wxEXPAND, 5);

    auto temperatureDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Photo-grade range is 1000K - 40000K. White light = 6500K.\nRange can be limited by Windows, see README for details"));
    temperatureBrightnessSizer->AddSpacer((10, 10)); 
    temperatureBrightnessSizer->Add(temperatureDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    // Spacer row
    temperatureBrightnessSizer->AddSpacer((10, 20));
    temperatureBrightnessSizer->AddSpacer((10, 20));

    auto brightnessText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Brightness"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->Add(brightnessText, 0, wxALL, 5);

    brightnessSlider_ = new wxSlider(colorControlsBox->GetStaticBox(), BRIGHTNESS_SLIDER,
        kDefaultBrightness - 128, kMinBrightness - 128, kMaxBrightness - 128, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(brightnessSlider_, 0, wxALL | wxEXPAND, 5);

    auto brightnessDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Use brightness slider to tweak color filtering.\nIt controls red/orange - blue/white balance depending on temperature"), wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->AddSpacer((10, 10)); 
    temperatureBrightnessSizer->Add(brightnessDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);


    // Time and apply
    wxBoxSizer* timeSizer = new wxBoxSizer(wxHORIZONTAL);

    auto turnOnText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Turn on at"));
    timeSizer->Add(turnOnText, 0, wxALIGN_CENTER | wxALL, 5);

    timePicker_ = new wxTimePickerCtrl(colorControlsBox->GetStaticBox(), wxID_ANY);
    timeSizer->Add(timePicker_, 0, wxALIGN_CENTER | wxALL, 5);
    timeSizer->Add(0, 0, 1, wxEXPAND, 5);
    auto apply = new wxButton(colorControlsBox->GetStaticBox(), BN_APPLY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0);
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
    decTemperature_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    auto decColorTempApply = new wxButton(hotkeysBox->GetStaticBox(), DEC_TEMPERATURE_APPLY, _("Apply"));
    auto decColorTempClear = new wxButton(hotkeysBox->GetStaticBox(), DEC_TEMPERATURE_CLEAR, _("Clear"));

    auto incColorTempText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase color temp"));
    incTemperature_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    auto incColorTempApply = new wxButton(hotkeysBox->GetStaticBox(), INC_TEMPERATURE_APPLY, _("Apply"));
    auto incColorTempClear = new wxButton(hotkeysBox->GetStaticBox(), INC_TEMPERATURE_CLEAR, _("Clear"));

    auto decBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Decrease brightness"));
    decBrightness_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    auto decBrightnessApply = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_APPLY, _("Apply"));
    auto decBrightnessClear = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_CLEAR, _("Clear"));

    auto incBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase Brightness"));
    incBrightness_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    auto incBrightnessApply = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_APPLY, _("Apply"));
    auto incBrightnessClear = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_CLEAR, _("Clear"));

    auto enableDisableText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Enable/Disable"));
    enableDisable_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    auto enableDisableApply = new wxButton(hotkeysBox->GetStaticBox(), ENABLE_DISABLE_APPLY, _("Apply"));
    auto enableDisableClear = new wxButton(hotkeysBox->GetStaticBox(), ENABLE_DISABLE_CLEAR, _("Clear"));

    auto hotkeysSizer = new wxFlexGridSizer(6, 4, 0, 0);
    hotkeysSizer->AddGrowableRow(5);
    hotkeysSizer->SetFlexibleDirection(wxBOTH);
    hotkeysSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    hotkeysSizer->Add(decColorTempText, 0, wxALL, 5);
    hotkeysSizer->Add(decTemperature_, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTempApply, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTempClear, 0, wxALL, 5);

    hotkeysSizer->Add(incColorTempText, 0, wxALL, 5);
    hotkeysSizer->Add(incTemperature_, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempApply, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempClear, 0, wxALL, 5);

    hotkeysSizer->Add(decBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightness_, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessApply, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessClear, 0, wxALL, 5);

    hotkeysSizer->Add(incBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightness_, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessApply, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessClear, 0, wxALL, 5);

    hotkeysSizer->Add(enableDisableText, 0, wxALL, 5);
    hotkeysSizer->Add(enableDisable_, 0, wxALL, 5);
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
    incTemperature_->SetHotkey(settings_.incTemperature);
    decTemperature_->SetHotkey(settings_.decTemperature);
    incBrightness_->SetHotkey(settings_.incBrightness);
    decBrightness_->SetHotkey(settings_.decBrightness);
    enableDisable_->SetHotkey(settings_.enableDisable);


    
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




void MainWindow::ApplyIncTemperatureHotkey(wxCommandEvent& event) {
    settings_.incTemperature = incTemperature_->GetHotkey();
    RegisterHotKey(Hotkeys::INC_TEMPERATURE, settings_.incTemperature.GetModifiers(), settings_.incTemperature.GetKey());
}

void MainWindow::ApplyDecTemperatureHotkey(wxCommandEvent& event) {
    settings_.decTemperature = decTemperature_->GetHotkey();
    RegisterHotKey(Hotkeys::DEC_TEMPERATURE, settings_.decTemperature.GetModifiers(), settings_.decTemperature.GetKey());
}

void MainWindow::ApplyIncBrightnessHotkey(wxCommandEvent& event) {
    settings_.incBrightness = incBrightness_->GetHotkey();
    RegisterHotKey(Hotkeys::INC_BRIGHTNESS, settings_.incBrightness.GetModifiers(), settings_.incBrightness.GetKey());
}

void MainWindow::ApplyDecBrightnessHotkey(wxCommandEvent& event) {
    settings_.decBrightness = decBrightness_->GetHotkey();
    RegisterHotKey(Hotkeys::DEC_BRIGHTNESS, settings_.decBrightness.GetModifiers(), settings_.decBrightness.GetKey());
}

void MainWindow::ApplyEnableDisableHotkey(wxCommandEvent& event) {
    settings_.enableDisable = enableDisable_->GetHotkey();
    RegisterHotKey(Hotkeys::ENABLE_DISABLE, settings_.enableDisable.GetModifiers(), settings_.enableDisable.GetKey());

}


void MainWindow::ClearIncTemperatureHotkey(wxCommandEvent& event) {
    incTemperature_->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_TEMPERATURE);
}

void MainWindow::ClearDecTemperatureHotkey(wxCommandEvent& event) {
    decTemperature_->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_TEMPERATURE);
}

void MainWindow::ClearIncBrightnessHotkey(wxCommandEvent& event) {
    incBrightness_->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_BRIGHTNESS);
}

void MainWindow::ClearDecBrightnessHotkey(wxCommandEvent& event) {
    decBrightness_->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_BRIGHTNESS);
}

void MainWindow::ClearEnableDisableHotkey(wxCommandEvent& event) {
    enableDisable_->ClearHotkey();
    UnregisterHotKey(Hotkeys::ENABLE_DISABLE);
}


void MainWindow::OnHotkey(wxKeyEvent& event) {
    const auto hotkey = Hotkey{event.GetKeyCode(), event.GetModifiers(), event.GetUnicodeKey()};

    if (hotkey == settings_.incTemperature) {
        IncreaseTemperature();
    } else if (hotkey == settings_.decTemperature) {
        DecreaseTemperature();
    } else if (hotkey == settings_.incBrightness) {
        IncreaseBrightness();
    } else if (hotkey == settings_.decBrightness) {
        DecreaseBrightness();
    } else if (hotkey == settings_.enableDisable) {
        DefaultBrightness();
    }
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

void MainWindow::IncreaseTemperature() {
    current_.temperatureK += kTemperatureStep;
    if (current_.temperatureK > kMaxTemperatureK)
        current_.temperatureK = kMaxTemperatureK;
    Ramp2(current_);
    UpdateSliders();
}

void MainWindow::DecreaseTemperature() {
    current_.temperatureK -= kTemperatureStep;
    if (current_.temperatureK < kMinTemperatureK)
        current_.temperatureK = kMinTemperatureK;
    Ramp2(current_);
    UpdateSliders();
}

void MainWindow::IncreaseBrightness() {
    current_.brightness += kBrightnessStep;
    if (current_.brightness > kMaxBrightness)
        current_.brightness = kMaxBrightness;
    Ramp2(current_);
    UpdateSliders();
}

void MainWindow::DecreaseBrightness() {
    current_.brightness -= kBrightnessStep;
    if (current_.brightness < kMinBrightness)
        current_.brightness = kMinBrightness;
    Ramp2(current_);
    UpdateSliders();
}

void MainWindow::DefaultBrightness() {
    current_ = ColorSettings{};
    Ramp(current_);
    UpdateSliders();
}

void MainWindow::OnTemperatureSlider(wxCommandEvent& event) {
    current_.temperatureK = event.GetInt();
    Ramp2(current_);
}

void MainWindow::OnBrightnessSlider(wxCommandEvent& event) {
    current_.brightness = event.GetInt() + 128;
    Ramp2(current_);
}

void MainWindow::UpdateSliders() {
    temperatureSlider_->SetValue(current_.temperatureK);
    brightnessSlider_->SetValue(current_.brightness - 128);
}

void MainWindow::OnApply(wxCommandEvent& event) {
    (daySelect_->GetValue() ? settings_.dayColors : settings_.nightColors) = current_;
}
void MainWindow::SwitchToDay(wxCommandEvent& event) {
    current_ = settings_.dayColors;
    Ramp2(current_);
    UpdateSliders();
}
void MainWindow::SwitchToNight(wxCommandEvent& event) {
    current_ = settings_.nightColors;
    Ramp2(current_);
    UpdateSliders();
}

/*
 void MainWindow::onResize(wxSizeEvent& event) {
    // layout everything in the dialog
    // temperatureDescription->Wrap(event.GetSize().GetWidth());
    Layout();

    event.Skip();
}
*/
