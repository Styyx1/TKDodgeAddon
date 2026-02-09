#include "ui.h"
#include "Settings.h"
#include "Utility.h"

namespace Menu
{
void RegisterDodgeMenu()
{
    if (!SKSEMenuFramework::IsInstalled())
    {
        return;
    }
    SKSEMenuFramework::SetSection(Titles::MOD_TITLE);
    SKSEMenuFramework::AddSectionItem(Titles::SETTINGS_SEC, Settings::RenderSettings);
    SKSEMenuFramework::AddInputEvent(Settings::OnInput);
    RestoreFromSettings();
}
void RestoreFromSettings()
{
    using set = Config::Settings;
    using namespace Menu::Settings::Var;

    enable_sneak_key_dodge = set::enable_sneak_key_dodge.GetValue();
    enable_dodge_in_place = set::enable_dodge_in_place.GetValue();
    step_dodge = set::step_dodge.GetValue();
    enable_sneak_dodge = set::enable_sneak_dodge.GetValue();
    enable_dodge_attack_cancel = set::enable_dodge_attack_cancel.GetValue();
    only_cancel_light = set::only_cancel_light.GetValue();
    i_frame_duration = set::i_frame_duration.GetValue();
    default_dodge_event = set::default_dodge_event.GetValue();
    sprinting_press_duration = set::sprinting_press_duration.GetValue();
    sneaking_press_duration = set::sneaking_press_duration.GetValue();

    dodge_cost = set::dodge_cost.GetValue();
    dodge_key = set::dodge_key.GetValue();
    use_sprint_key = set::use_sprint_key.GetValue();
    use_mco_recover_window = set::use_mco_recover_window.GetValue();

    use_perk_lock = set::use_perk_lock.GetValue();
    use_percentage_cost = set::use_percentage_cost.GetValue();

    use_double_tap = set::use_double_tap.GetValue();
}
void ResetDefaults()
{
    using set = Config::Settings;
    using namespace Menu::Settings::Var;

    enable_sneak_key_dodge = false;
    enable_dodge_in_place = true;
    step_dodge = false;
    enable_sneak_dodge = false;
    enable_dodge_attack_cancel = true;
    use_sprint_key = false;
    use_mco_recover_window = false;
    use_perk_lock = false;
    use_percentage_cost = false;
    use_double_tap = false;
    only_cancel_light = false;

    i_frame_duration = 0.3f;
    sprinting_press_duration = 0.5f;
    sneaking_press_duration = 0.5f;
    dodge_cost = 15.0f;

    dodge_perk_form_ID = 0;
    on_dodge_spell_form_ID = 0;
    spell_lock_perk_form_ID = 0;

    default_dodge_event = "TKDodgeForward";
    dodge_key = 274;
    perk_mod_name = "";
    spell_mod_name = "";

    set::enable_sneak_key_dodge.SetValue(enable_sneak_key_dodge);
    set::enable_dodge_in_place.SetValue(enable_dodge_in_place);
    set::step_dodge.SetValue(step_dodge);
    set::enable_sneak_dodge.SetValue(enable_sneak_dodge);
    set::enable_dodge_attack_cancel.SetValue(enable_dodge_attack_cancel);
    set::only_cancel_light.SetValue(only_cancel_light);
    set::i_frame_duration.SetValue(i_frame_duration);
    set::default_dodge_event.SetValue(default_dodge_event);
    set::sprinting_press_duration.SetValue(sprinting_press_duration);
    set::sneaking_press_duration.SetValue(sneaking_press_duration);

    set::dodge_cost.SetValue(dodge_cost);
    set::dodge_key.SetValue(dodge_key);
    set::use_sprint_key.SetValue(use_sprint_key);
    set::use_mco_recover_window.SetValue(use_mco_recover_window);

    set::use_perk_lock.SetValue(use_perk_lock);
    set::use_percentage_cost.SetValue(use_percentage_cost);

    set::use_double_tap.SetValue(use_double_tap);

    // Save Settings
    set::UpdateSettings(true);
}
void RenderSystem()
{
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Label::system.c_str());

    if (ImGuiMCP::Button(Label::save_settings.c_str()))
    {
        Config::Settings::UpdateSettings(true);
    }

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
    {
        ResetDefaults();
    }
}
} // namespace Menu

void Menu::Settings::DrawHotkeyConfigUI()
{
    std::string key_name = hotkeys::details::GetNameByKey(Config::Settings::dodge_key.GetValue()).data();

    // not really needed but looks better in the menu
    std::transform(key_name.begin(), key_name.end(), key_name.begin(), ::toupper);

    ImGuiMCP::Text(std::format("Hotkey: {}", key_name).c_str());
    ImGuiMCP::SameLine();

    if (!Menu::Settings::capture_key_input)
    {
        if (ImGuiMCP::Button("Rebind"))
        {
            Menu::Settings::capture_key_input = true;
            Menu::Settings::Var::dodge_key = 0;
        }
        ImGuiMCP::SameLine();
        ux::HelpMarker("Press the desired key to rebind the visibility toggle.");
    }
    else
    {
        ImGuiMCP::Text("Press any key");
        ImGuiMCP::SameLine();
        if (ImGuiMCP::Button("Cancel"))
        {
            Menu::Settings::capture_key_input = false;
            Settings::Var::dodge_key = Config::Settings::dodge_key.GetValue();
        }
    }
    if (Menu::Settings::Var::dodge_key != 0)
    {
        Config::Settings::dodge_key.SetValue(Menu::Settings::Var::dodge_key);
        Menu::Settings::capture_key_input = false;
    }
}

bool __stdcall Menu::Settings::OnInput(RE::InputEvent* event)
{
    bool blockThisUserInput = false;

    if (Menu::Settings::capture_key_input)
    {
        for (auto e = event; e; e = e->next)
        {
            const auto button = e->AsButtonEvent();
            if (!button || !button->HasIDCode())
                continue;

            if (!button->IsDown())
                continue;

            uint32_t key = button->GetIDCode();

            switch (button->GetDevice()) {
            case RE::INPUT_DEVICE::kMouse:
                key += SKSE::InputMap::kMacro_MouseButtonOffset;
                break;
            case RE::INPUT_DEVICE::kGamepad:
                key = SKSE::InputMap::GamepadMaskToKeycode(key);
                break;
            default:
                break;
            }
            Menu::Settings::Var::dodge_key = key;
            blockThisUserInput = true;
        }

    }
    return blockThisUserInput;
}


void __stdcall Menu::Settings::RenderSettings()
{
    using set = Config::Settings;
    FontAwesome::PushSolid();
    ImGuiMCP::Text(Menu::Titles::MOD_TITLE.c_str());
    ImGuiMCP::NewLine();

    SettingCheckbox(Label::enable_sneak_key_dodge.c_str(), Var::enable_sneak_key_dodge, set::enable_sneak_key_dodge,
                    Tool::enable_sneak_key_dodge.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::enable_dodge_in_place.c_str(), Var::enable_dodge_in_place, set::enable_dodge_in_place,
                    Tool::enable_dodge_in_place.c_str());

    SettingCheckbox(Label::step_dodge.c_str(), Var::step_dodge, set::step_dodge, Tool::step_dodge.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::enable_sneak_dodge.c_str(), Var::enable_sneak_dodge, set::enable_sneak_dodge,
                    Tool::enable_sneak_dodge.c_str());

    SettingCheckbox(Label::enable_dodge_attack_cancel.c_str(), Var::enable_dodge_attack_cancel,
                    set::enable_dodge_attack_cancel, Tool::enable_dodge_attack_cancel.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::use_sprint_key.c_str(), Var::use_sprint_key, set::use_sprint_key,
                    Tool::use_sprint_key.c_str());

    SettingCheckbox(Label::use_mco_recover_window.c_str(), Var::use_mco_recover_window, set::use_mco_recover_window,
                    Tool::use_mco_recover_window.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::use_perk_lock.c_str(), Var::use_perk_lock, set::use_perk_lock, Tool::use_perk_lock.c_str());

    SettingCheckbox(Label::use_percentage_cost.c_str(), Var::use_percentage_cost, set::use_percentage_cost,
                    Tool::use_percentage_cost.c_str());
    ImGuiMCP::SameLine();
    SettingCheckbox(Label::use_double_tap.c_str(), Var::use_double_tap, set::use_double_tap, Tool::use_double_tap.c_str());

    SettingCheckbox(Label::only_cancel_light.c_str(), Var::only_cancel_light, set::only_cancel_light,Tool::only_cancel_light.c_str());


    SettingSlider(Label::i_frame_duration.c_str(), Var::i_frame_duration, 0.0f, 4.0f, "%.2f sec", set::i_frame_duration,
                  Tool::i_frame_duration.c_str());
    SettingSlider(Label::sprinting_press_duration.c_str(), Var::sprinting_press_duration, 0.01f, 4.0f, "%.2f sec",
                  set::sprinting_press_duration, Tool::sprinting_press_duration.c_str());
    SettingSlider(Label::sneaking_press_duration.c_str(), Var::sneaking_press_duration, 0.01f, 4.0f, "%.2f sec",
                  set::sneaking_press_duration, Tool::sneaking_press_duration.c_str());
    SettingSlider(Label::dodge_cost.c_str(), Var::dodge_cost, 0.0f, 100.0f, "%.2f", set::dodge_cost,
                  Tool::dodge_cost.c_str());

    DrawHotkeyConfigUI();
    RenderSystem();
    
    FontAwesome::Pop();
}
