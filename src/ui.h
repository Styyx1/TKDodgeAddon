#pragma once
#include <st-ui.h>

namespace Menu
{
    namespace Titles
    {
        inline std::string MOD_TITLE = "TK Dodge RE Addon";
        inline std::string SETTINGS_SEC = "Settings";
    } // namespace Titles
    namespace Label
    {
        inline std::string system = FontAwesome::UnicodeToUtf8(0xf390) + " System";
        inline std::string save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
        inline std::string restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";
    } // namespace Label
    void RegisterDodgeMenu();
    void RestoreFromSettings();
    void ResetDefaults();
    void RenderSystem();

    using ux = StyyxUtil::UIUtil;
    template <class T>
    bool SettingSlider(const char *label, T &slider_var, const T min, const T max, const char *fmt,
                       REX::TOML::Setting<T> &setting, const char *help)
    {
        ImGuiMCP::SetNextItemWidth(200.f);
        const bool changed = ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_Float, &slider_var,
                                                     &min, &max, fmt);
        if (changed)
        {
            setting.SetValue(slider_var);
        }
        ImGuiMCP::SameLine();
        ux::HelpMarker(help);
        return changed;
    }

    template <class T>
    bool SettingSliderINT(const char *label, T &slider_var, const T min, const T max, const char *fmt,
                          REX::TOML::Setting<T> &setting, const char *help)
    {

        ImGuiMCP::SetNextItemWidth(200.f);
        const bool changed =
            ImGuiMCP::SliderScalar(label, ImGuiMCP::ImGuiDataType_::ImGuiDataType_S32, &slider_var, &min, &max, fmt);
        if (changed)
        {
            setting.SetValue(slider_var);
        }
        ImGuiMCP::SameLine();
        ux::HelpMarker(help);
        return changed;
    }

    inline bool SettingCheckbox(const char *label, bool &value_var, REX::TOML::Setting<bool> &setting, const char *help)
    {
        const bool changed = ImGuiMCP::Checkbox(label, &value_var);
        if (changed)
        {
            setting.SetValue(value_var);
        }
        ImGuiMCP::SameLine();
        ux::HelpMarker(help);
        return changed;
    }

    namespace Settings
    {
        inline bool capture_key_input = false;
        namespace Titles
        {

        }
        namespace Label
        {
            inline std::string enable_sneak_key_dodge = "Enable Dodge with Sneak Key";
            inline std::string enable_dodge_in_place = "Enable Dodge in Place";
            inline std::string step_dodge = "Toggle Step Dodge";
            inline std::string enable_sneak_dodge = "Enable Dodge while Sneaking";
            inline std::string enable_dodge_attack_cancel = "Dodge to cancel Attacks";
            inline std::string i_frame_duration = "iFrame Duration";
            inline std::string default_dodge_event = "Default Dodge Event";
            inline std::string sprinting_press_duration = "Sprint Key Press Duration";
            inline std::string sneaking_press_duration = "Sneak Key Press Duration";

            inline std::string dodge_cost = "Dodge Cost";
            inline std::string dodge_key = "Dodge Key";
            inline std::string use_sprint_key = "Use Sprint Key";
            inline std::string use_mco_recover_window = "Only Dodge when MCO Recover Window is Open";

            inline std::string use_perk_lock = "Lock Dodge behind Perk";
            inline std::string use_percentage_cost = "Dodge Cost is Percentage";

            inline std::string use_double_tap = "Double tab Dodge";

            inline std::string perk_mod_name = "Mod Name for Dodge Perks";
            inline std::string spell_mod_name = "Mod Name for OnDodge Spell";

            inline std::string dodge_perk_form_ID = "Dodge Perk Form ID";
            inline std::string on_dodge_spell_form_ID = "OnDodge Spell Form ID";
            inline std::string spell_lock_perk_form_ID = "OnDodge Spell Lock Perk Form ID";
        }
        namespace Tool
        {
            inline std::string enable_sneak_key_dodge = "Toggle Doding with Sneak Key";
            inline std::string enable_dodge_in_place = "Toggle Dodge in Place";
            inline std::string step_dodge = "Toggle Step Dodge";
            inline std::string enable_sneak_dodge = "Toggle Dodge while Sneaking";
            inline std::string enable_dodge_attack_cancel = "Toggle Dodge to cancel Attacks";
            inline std::string i_frame_duration = "Duration of invincibilty frames";
            inline std::string default_dodge_event = "Set the default dodge Event";
            inline std::string sprinting_press_duration = "Minimum duration of sprint key press to not dodge and sprint instead";
            inline std::string sneaking_press_duration = "Minimkum duration of sneak key press to not dodge and sneak instead";
            inline std::string use_double_tap = "Double Tab Dodge/Sneak/Sprint key to dodge";

            inline std::string dodge_cost = "Stamina const for dodging";
            inline std::string dodge_key = "Key to dodge";
            inline std::string use_sprint_key = "Toggle Dodge with Sprint Key";
            inline std::string use_mco_recover_window = "Toggle Dodge only when MCO Recover Window is Open";

            inline std::string use_perk_lock = "Toggle Dodging requires a Perk";
            inline std::string use_percentage_cost =
                "Stamina cost for Dodging uses a percentage of max Stamina instead of a flat value";

            inline std::string perk_mod_name = "Name of th mod for Dodge Perks";
            inline std::string spell_mod_name = "Name of the mod for OnDodge Spell";

            inline std::string dodge_perk_form_ID = "FormID for dodge perk. Needs to be in this format: 0x800";
            inline std::string on_dodge_spell_form_ID = "FormID for OnDodge Spell, needs to be in this format: 0x800";
            inline std::string spell_lock_perk_form_ID = "FormID for Spell Lock Perk, needs to be in this format: 0x800";
        }
        namespace Var
        {

            inline bool enable_sneak_key_dodge;
            inline bool enable_dodge_in_place;
            inline bool step_dodge;
            inline bool enable_sneak_dodge;
            inline bool enable_dodge_attack_cancel;
            inline float i_frame_duration;
            inline std::string default_dodge_event;
            inline float sprinting_press_duration;
            inline float sneaking_press_duration;

            inline float dodge_cost;
            inline uint32_t dodge_key;
            inline bool use_sprint_key;
            inline bool use_mco_recover_window;
            inline bool only_light_attack_cancel;

            inline bool use_perk_lock;
            inline bool use_percentage_cost;

            inline std::string perk_mod_name;
            inline std::string spell_mod_name;

            inline uint32_t dodge_perk_form_ID;
            inline uint32_t on_dodge_spell_form_ID;
            inline uint32_t spell_lock_perk_form_ID;
            inline bool use_double_tap;

        } // namespace Var
        inline void DrawHotkeyConfigUI();
        bool __stdcall OnInput(RE::InputEvent* event);
        void __stdcall RenderSettings();

    } // namespace Settings
} // namespace Menu
