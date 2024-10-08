#include "main_window.h"

#include "color_correction.h"
#include "hotkey_input.h"

#include <chrono>

namespace {

const std::string kSettingsFileName = "settings.json";
inline const auto kColorUpdateInterval = std::chrono::seconds(5);

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

    BN_RESET,
    BN_APPLY,
    RB_DAY,
    RB_NIGHT,

    TEMPERATURE_SLIDER,
    BRIGHTNESS_SLIDER,
    
    COLORS_UPDATE_TIMER,
};

enum Hotkeys {
    DEC_TEMPERATURE = 0,
    INC_TEMPERATURE,
    DEC_BRIGHTNESS,
    INC_BRIGHTNESS,
    ENABLE_DISABLE
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

    EVT_BUTTON(BN_RESET, MainWindow::OnReset)
    EVT_BUTTON(BN_APPLY, MainWindow::OnApply)
    EVT_RADIOBUTTON(RB_DAY, MainWindow::SwitchToDay)
    EVT_RADIOBUTTON(RB_NIGHT, MainWindow::SwitchToNight)

    EVT_SLIDER(TEMPERATURE_SLIDER, MainWindow::OnTemperatureSlider)
    EVT_SLIDER(BRIGHTNESS_SLIDER, MainWindow::OnBrightnessSlider)

    EVT_CLOSE(MainWindow::OnCloseWindow)
    EVT_ICONIZE(MainWindow::OnIconize)
    EVT_HOTKEY(wxID_ANY, MainWindow::OnHotkey)
    EVT_TIMER(COLORS_UPDATE_TIMER, MainWindow::UpdateColorsOnTimer)
wxEND_EVENT_TABLE()
// clang-format on

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxMINIMIZE_BOX )
    , taskbarIcon_{std::make_unique<TaskbarIcon>(this)} {

    wxIcon icon(wxIconLocation(R"(day_and_night.ico)"));
    if (icon.IsOk()) {
        SetIcon(icon);
        if (!taskbarIcon_->SetIcon(icon, "SoftNight color control")) {
            wxLogError("Could not set icon.");
        }
    }

    SetupUi();

    settings_ = LoadSettings(kSettingsFileName);

    UpdateColorControls();
    UpdateHotkeysFields();
    UpdateTimeField();
    UpdateSwitchColorInfo(&settings_.activeColors);
    if (settings_.activeColors == settings_.dayColors) {
        wxCommandEvent e{};
        SwitchToDay(e);
        daySelect_->SetFocus();
    } else {
        wxCommandEvent e{};
        SwitchToNight(e);
        nightSelect_->SetFocus();
    }

    RegisterHotKeys();

    colorTimer_ = std::make_unique<wxTimer>(this, COLORS_UPDATE_TIMER);
    colorTimer_->SetOwner(this, COLORS_UPDATE_TIMER);
    StartUpdateColorsTimer();
}


MainWindow::~MainWindow() {
    StopUpdateColorsTimer();
#ifdef _DEBUG
    // Visual studio bug
    // This prevents the tzdb allocations from being reported as memory leaks
    std::chrono::get_tzdb_list().~tzdb_list();
#endif
}

void MainWindow::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {
    Destroy();
}

void MainWindow::SetupUi() {
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxFlexGridSizer* mainFlexSizer = new wxFlexGridSizer(2, 1, 0, 0);
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

    auto temperatureText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Temperature, K"));
    temperatureBrightnessSizer->Add(temperatureText, 0, wxALL, 5);

    temperatureSlider_ = new wxSlider(colorControlsBox->GetStaticBox(), TEMPERATURE_SLIDER, kDefaultTemperatureK, kMinTemperatureK,
                     kMaxTemperatureK, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(temperatureSlider_, 0, wxALL | wxEXPAND, 5);

    auto temperatureDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY,
                                                   _("Photo-grade range is 1000K - 40000K. White light = 6500K\nRange "
                                                     "can be limited by Windows, see README.md for details"));
    temperatureBrightnessSizer->AddSpacer((10, 10));
    temperatureBrightnessSizer->Add(temperatureDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    // Spacer row
    temperatureBrightnessSizer->AddSpacer((10, 20));
    temperatureBrightnessSizer->AddSpacer((10, 20));

    auto brightnessText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Brightness"));
    temperatureBrightnessSizer->Add(brightnessText, 0, wxALL, 5);

    brightnessSlider_ = new wxSlider(colorControlsBox->GetStaticBox(), BRIGHTNESS_SLIDER, kDefaultBrightness - 128,
                                     kMinBrightness - 128, kMaxBrightness - 128, wxDefaultPosition, wxDefaultSize,
                                     wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    temperatureBrightnessSizer->Add(brightnessSlider_, 0, wxALL | wxEXPAND, 5);

    auto brightnessDescription = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY,
                                                  _("Use brightness slider to tweak color filtering\nIt controls "
                                                    "red/orange - blue/white balance depending on temperature"),
                                                  wxDefaultPosition, wxDefaultSize, 0);
    temperatureBrightnessSizer->AddSpacer((10, 10));
    temperatureBrightnessSizer->Add(brightnessDescription, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    // Time and apply
    wxBoxSizer* timeSizer = new wxBoxSizer(wxHORIZONTAL);

    auto turnOnText = new wxStaticText(colorControlsBox->GetStaticBox(), wxID_ANY, _("Turn on at"));
    timeSizer->Add(turnOnText, 0, wxALIGN_CENTER | wxALL, 5);

    timePicker_ = new wxTimePickerCtrl(colorControlsBox->GetStaticBox(), wxID_ANY);
    timeSizer->Add(timePicker_, 0, wxALIGN_CENTER | wxALL, 5);
    timeSizer->Add(0, 0, 1, wxEXPAND, 5);

    resetButton_ = new wxButton(colorControlsBox->GetStaticBox(), BN_RESET, _("Reset"));
    timeSizer->Add(resetButton_, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    applyButton_ = new wxButton(colorControlsBox->GetStaticBox(), BN_APPLY, _("Apply"));
    timeSizer->Add(applyButton_, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

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
    decColorTempApply_ = new wxButton(hotkeysBox->GetStaticBox(), DEC_TEMPERATURE_APPLY, _("Apply"));
    decColorTempClear_ = new wxButton(hotkeysBox->GetStaticBox(), DEC_TEMPERATURE_CLEAR, _("Clear"));

    auto incColorTempText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase color temp"));
    incTemperature_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    incColorTempApply_ = new wxButton(hotkeysBox->GetStaticBox(), INC_TEMPERATURE_APPLY, _("Apply"));
    incColorTempClear_ = new wxButton(hotkeysBox->GetStaticBox(), INC_TEMPERATURE_CLEAR, _("Clear"));

    auto decBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Decrease brightness"));
    decBrightness_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    decBrightnessApply_ = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_APPLY, _("Apply"));
    decBrightnessClear_ = new wxButton(hotkeysBox->GetStaticBox(), DEC_BRIGHTNESS_CLEAR, _("Clear"));

    auto incBrightnessText = new wxStaticText(hotkeysBox->GetStaticBox(), wxID_ANY, _("Increase Brightness"));
    incBrightness_ = new HotkeyInput(hotkeysBox->GetStaticBox(), wxID_ANY);
    incBrightnessApply_ = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_APPLY, _("Apply"));
    incBrightnessClear_ = new wxButton(hotkeysBox->GetStaticBox(), INC_BRIGHTNESS_CLEAR, _("Clear"));

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
    hotkeysSizer->Add(decColorTempApply_, 0, wxALL, 5);
    hotkeysSizer->Add(decColorTempClear_, 0, wxALL, 5);

    hotkeysSizer->Add(incColorTempText, 0, wxALL, 5);
    hotkeysSizer->Add(incTemperature_, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempApply_, 0, wxALL, 5);
    hotkeysSizer->Add(incColorTempClear_, 0, wxALL, 5);

    hotkeysSizer->Add(decBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightness_, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessApply_, 0, wxALL, 5);
    hotkeysSizer->Add(decBrightnessClear_, 0, wxALL, 5);

    hotkeysSizer->Add(incBrightnessText, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightness_, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessApply_, 0, wxALL, 5);
    hotkeysSizer->Add(incBrightnessClear_, 0, wxALL, 5);

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

    this->SetSize({650, 700});
    this->Layout();
    this->Centre(wxBOTH);
}

void MainWindow::UpdateSwitchColorInfo(ColorSettings* oppositeColorSettings) {
    using days = std::chrono::duration<int, std::ratio<86400>>;

    const auto curTime = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()};
    const auto lastMidnight = std::chrono::time_point_cast<days>(curTime.get_local_time());

    auto switchToDay = lastMidnight + std::chrono::hours{settings_.swithToDay.hour} + std::chrono::minutes{settings_.swithToDay.minute} + std::chrono::seconds{settings_.swithToDay.second};
    auto switchToNight = lastMidnight + std::chrono::hours{settings_.swithToNight.hour} + std::chrono::minutes{settings_.swithToNight.minute} + std::chrono::seconds{settings_.swithToNight.second};

    if (switchToDay < curTime.get_local_time()) {
        switchToDay += std::chrono::hours{24};
    }

    if (switchToNight < curTime.get_local_time()) {
        switchToNight += std::chrono::hours{24};
    }

    if (switchToDay < switchToNight) {
        switchColorInfo_.epochTimeToSwitch = switchToDay.time_since_epoch() / std::chrono::milliseconds(1);
        switchColorInfo_.switchTo = SwitchColorInfo::SwitchTo::Day;
        if (oppositeColorSettings)
            *oppositeColorSettings = settings_.nightColors;
    } else {
        switchColorInfo_.epochTimeToSwitch = switchToNight.time_since_epoch() / std::chrono::milliseconds(1);
        switchColorInfo_.switchTo = SwitchColorInfo::SwitchTo::Night;
        if (oppositeColorSettings)
            *oppositeColorSettings = settings_.dayColors;
    }
}

void MainWindow::StartUpdateColorsTimer() {
    colorTimer_->Start(kColorUpdateInterval.count());
}

void MainWindow::StopUpdateColorsTimer() {
    colorTimer_->Stop();
}

void MainWindow::UpdateColorsOnTimer(wxTimerEvent& WXUNUSED(event)) {
    const auto curTime = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()};

    if (curTime.get_local_time().time_since_epoch() / std::chrono::milliseconds(1) >= switchColorInfo_.epochTimeToSwitch) {
        wxCommandEvent e{};
        if (switchColorInfo_.switchTo == SwitchColorInfo::SwitchTo::Day)
            SwitchToDay(e);
        else
            SwitchToNight(e);

        UpdateSwitchColorInfo();
    }
}

void MainWindow::RegisterHotKeys() {
    RegisterHotKey(Hotkeys::DEC_TEMPERATURE, settings_.decTemperature.GetModifiers(), settings_.decTemperature.GetKey());
    RegisterHotKey(Hotkeys::INC_TEMPERATURE, settings_.incTemperature.GetModifiers(), settings_.incTemperature.GetKey());
    RegisterHotKey(Hotkeys::DEC_BRIGHTNESS, settings_.decBrightness.GetModifiers(), settings_.decBrightness.GetKey());
    RegisterHotKey(Hotkeys::INC_BRIGHTNESS, settings_.incBrightness.GetModifiers(), settings_.incBrightness.GetKey());
    RegisterHotKey(Hotkeys::ENABLE_DISABLE, settings_.enableDisable.GetModifiers(), settings_.enableDisable.GetKey());
}

void MainWindow::UnregisterHotKeys() {
    UnregisterHotKey(Hotkeys::DEC_TEMPERATURE);
    UnregisterHotKey(Hotkeys::INC_TEMPERATURE);
    UnregisterHotKey(Hotkeys::DEC_BRIGHTNESS);
    UnregisterHotKey(Hotkeys::INC_BRIGHTNESS);
}

void MainWindow::UpdateHotkeysFields() {
    incTemperature_->SetHotkey(settings_.incTemperature);
    decTemperature_->SetHotkey(settings_.decTemperature);
    incBrightness_->SetHotkey(settings_.incBrightness);
    decBrightness_->SetHotkey(settings_.decBrightness);
    enableDisable_->SetHotkey(settings_.enableDisable);
}

void MainWindow::ApplyIncTemperatureHotkey(wxCommandEvent& WXUNUSED(event)) {
    settings_.incTemperature = incTemperature_->GetHotkey();
    UnregisterHotKey(Hotkeys::INC_TEMPERATURE);
    RegisterHotKey(Hotkeys::INC_TEMPERATURE, settings_.incTemperature.GetModifiers(), settings_.incTemperature.GetKey());
    SaveSettings(settings_, kSettingsFileName);
}

void MainWindow::ApplyDecTemperatureHotkey(wxCommandEvent& WXUNUSED(event)) {
    settings_.decTemperature = decTemperature_->GetHotkey();
    UnregisterHotKey(Hotkeys::DEC_TEMPERATURE);
    RegisterHotKey(Hotkeys::DEC_TEMPERATURE, settings_.decTemperature.GetModifiers(), settings_.decTemperature.GetKey());
    SaveSettings(settings_, kSettingsFileName);
}

void MainWindow::ApplyIncBrightnessHotkey(wxCommandEvent& WXUNUSED(event)) {
    settings_.incBrightness = incBrightness_->GetHotkey();
    UnregisterHotKey(Hotkeys::INC_BRIGHTNESS);
    RegisterHotKey(Hotkeys::INC_BRIGHTNESS, settings_.incBrightness.GetModifiers(), settings_.incBrightness.GetKey());
    SaveSettings(settings_, kSettingsFileName);
}

void MainWindow::ApplyDecBrightnessHotkey(wxCommandEvent& WXUNUSED(event)) {
    settings_.decBrightness = decBrightness_->GetHotkey();
    UnregisterHotKey(Hotkeys::DEC_BRIGHTNESS);
    RegisterHotKey(Hotkeys::DEC_BRIGHTNESS, settings_.decBrightness.GetModifiers(), settings_.decBrightness.GetKey());
    SaveSettings(settings_, kSettingsFileName);
}

void MainWindow::ApplyEnableDisableHotkey(wxCommandEvent& WXUNUSED(event)) {
    settings_.enableDisable = enableDisable_->GetHotkey();
    UnregisterHotKey(Hotkeys::ENABLE_DISABLE);
    RegisterHotKey(Hotkeys::ENABLE_DISABLE, settings_.enableDisable.GetModifiers(), settings_.enableDisable.GetKey());
    SaveSettings(settings_, kSettingsFileName);
}


void MainWindow::ClearIncTemperatureHotkey(wxCommandEvent& WXUNUSED(event)) {
    incTemperature_->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_TEMPERATURE);
}

void MainWindow::ClearDecTemperatureHotkey(wxCommandEvent& WXUNUSED(event)) {
    decTemperature_->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_TEMPERATURE);
}

void MainWindow::ClearIncBrightnessHotkey(wxCommandEvent& WXUNUSED(event)) {
    incBrightness_->ClearHotkey();
    UnregisterHotKey(Hotkeys::INC_BRIGHTNESS);
}

void MainWindow::ClearDecBrightnessHotkey(wxCommandEvent& WXUNUSED(event)) {
    decBrightness_->ClearHotkey();
    UnregisterHotKey(Hotkeys::DEC_BRIGHTNESS);
}

void MainWindow::ClearEnableDisableHotkey(wxCommandEvent& WXUNUSED(event)) {
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
        EnableDisable();
    }
}

void MainWindow::OnIconize(wxIconizeEvent& event) {
    if (event.IsIconized()) {
        Show(false);
    }
}

void MainWindow::IncreaseTemperature() {
    settings_.activeColors.temperatureK += kTemperatureStep;
    if (settings_.activeColors.temperatureK > kMaxTemperatureK)
        settings_.activeColors.temperatureK = kMaxTemperatureK;
    ApplyActiveColors();
}

void MainWindow::DecreaseTemperature() {
    settings_.activeColors.temperatureK -= kTemperatureStep;
    if (settings_.activeColors.temperatureK < kMinTemperatureK)
        settings_.activeColors.temperatureK = kMinTemperatureK;
    ApplyActiveColors();
}

void MainWindow::IncreaseBrightness() {
    settings_.activeColors.brightness += kBrightnessStep;
    if (settings_.activeColors.brightness > kMaxBrightness)
        settings_.activeColors.brightness = kMaxBrightness;
    ApplyActiveColors();
}

void MainWindow::DecreaseBrightness() {
    settings_.activeColors.brightness -= kBrightnessStep;
    if (settings_.activeColors.brightness < kMinBrightness)
        settings_.activeColors.brightness = kMinBrightness;
    ApplyActiveColors();
}

void MainWindow::EnableDisable() {
    settings_.isEnabled = !settings_.isEnabled;
    std::swap(settings_.activeColors, settings_.backupColors);
    if (settings_.isEnabled) {
        RegisterHotKeys();
        StartUpdateColorsTimer();
    } else {
        UnregisterHotKeys();
        StopUpdateColorsTimer();
    }
    ApplyActiveColors();
}

void MainWindow::OnTemperatureSlider(wxCommandEvent& event) {
    settings_.activeColors.temperatureK = event.GetInt();
    Ramp2(settings_.activeColors);
}

void MainWindow::OnBrightnessSlider(wxCommandEvent& event) {
    settings_.activeColors.brightness = event.GetInt() + 128;
    Ramp2(settings_.activeColors);
}

void MainWindow::UpdateColorControls() {
    temperatureSlider_->SetValue(settings_.activeColors.temperatureK);
    brightnessSlider_->SetValue(settings_.activeColors.brightness - 128);

    if (settings_.isEnabled) {
        temperatureSlider_->Enable();
        brightnessSlider_->Enable();
        daySelect_->Enable();
        nightSelect_->Enable();
        resetButton_->Enable();
        applyButton_->Enable();

        decTemperature_->Enable();
        decColorTempApply_->Enable();
        decColorTempClear_->Enable();
        incTemperature_->Enable();
        incColorTempApply_->Enable();
        incColorTempClear_->Enable();
        decBrightness_->Enable();
        decBrightnessApply_->Enable();
        decBrightnessClear_->Enable();
        incBrightness_->Enable();
        incBrightnessApply_->Enable();
        incBrightnessClear_->Enable();

        if (daySelect_->GetValue())
            daySelect_->SetFocus();
        else
            nightSelect_->SetFocus();

    } else {
        // Set focus on something else than hotkey input
        timePicker_->SetFocus();

        temperatureSlider_->Disable();
        brightnessSlider_->Disable();
        daySelect_->Disable();
        nightSelect_->Disable();
        resetButton_->Disable();
        applyButton_->Disable();

        decTemperature_->Disable();
        decColorTempApply_->Disable();
        decColorTempClear_->Disable();
        incTemperature_->Disable();
        incColorTempApply_->Disable();
        incColorTempClear_->Disable();
        decBrightness_->Disable();
        decBrightnessApply_->Disable();
        decBrightnessClear_->Disable();
        incBrightness_->Disable();
        incBrightnessApply_->Disable();
        incBrightnessClear_->Disable();
    }
}

void MainWindow::OnApply(wxCommandEvent& WXUNUSED(event)) {
    if (daySelect_->GetValue()) {
        settings_.dayColors = settings_.activeColors;
        timePicker_->GetTime(&settings_.swithToDay.hour, &settings_.swithToDay.minute, &settings_.swithToDay.second);
    } else {
        settings_.nightColors = settings_.activeColors;
        timePicker_->GetTime(&settings_.swithToNight.hour, &settings_.swithToNight.minute, &settings_.swithToNight.second);
    }

    SaveSettings(settings_, kSettingsFileName);
    UpdateSwitchColorInfo();
}

void MainWindow::OnReset(wxCommandEvent& WXUNUSED(event)) {
    settings_.activeColors = {kDefaultTemperatureK, kDefaultBrightness};
    ApplyActiveColors();
}

void MainWindow::SwitchToDay(wxCommandEvent& WXUNUSED(event)) {
    daySelect_->SetValue(true);
    settings_.activeColors = settings_.dayColors;
    timePicker_->SetTime(settings_.swithToDay.hour, settings_.swithToDay.minute, settings_.swithToDay.second);
    ApplyActiveColors();
}

void MainWindow::SwitchToNight(wxCommandEvent& WXUNUSED(event)) {
    nightSelect_->SetValue(true);
    settings_.activeColors = settings_.nightColors;
    timePicker_->SetTime(settings_.swithToNight.hour, settings_.swithToNight.minute, settings_.swithToNight.second);
    ApplyActiveColors();
}

void MainWindow::UpdateTimeField() {
    const auto& time = daySelect_->GetValue() ? settings_.swithToDay : settings_.swithToNight;
    timePicker_->SetTime(time.hour, time.minute, time.second);
}

void MainWindow::ApplyActiveColors() {
    Ramp2(settings_.activeColors);
    UpdateColorControls();
}