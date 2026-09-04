#pragma once

namespace Dodge
{

namespace MENU
{

namespace FUN
{

void RestoreDefaults();
void LoadFromConfig();

enum struct DefaultEvent
{
    kForward,
    kBack,
    kLeft,
    kRight,
};

inline std::string GetConfigFromEnum(DefaultEvent a_enum)
{
    switch (a_enum)
    {
        case DefaultEvent::kForward:
            return "TKDodgeForward";
        case DefaultEvent::kBack:
            return "TKDodgeBack";
        case DefaultEvent::kLeft:
            return "TKDodgeLeft";
        case DefaultEvent::kRight:
            return "TKDodgeRight";
        default:
            return "NONE";
    }
}


} // namespace FUN

namespace TIT
{
inline constexpr auto MENU_TITLE = "TK Dodge RE Addon";
inline constexpr auto KEY_SEC    = "Keybinds";
inline constexpr auto SETTINGS   = "Settings";
inline constexpr auto SYS_SEC    = "System";
} // namespace TIT
namespace LAB
{
inline constexpr auto enable_sneak_key_dodge     = "Enable Dodge with Sneak Key";
inline constexpr auto enable_dodge_in_place      = "Enable Dodge in Place";
inline constexpr auto step_dodge                 = "Toggle Step Dodge";
inline constexpr auto enable_sneak_dodge         = "Enable Dodge while Sneaking";
inline constexpr auto enable_dodge_attack_cancel = "Dodge to cancel Attacks";
inline constexpr auto only_cancel_light          = "Only Light Attack Cancel";
inline constexpr auto i_frame_duration           = "iFrame Duration";
inline constexpr auto default_dodge_event        = "Default Dodge Event";
inline constexpr auto sprinting_press_duration   = "Sprint Key Press Duration";
inline constexpr auto sneaking_press_duration    = "Sneak Key Press Duration";

inline constexpr auto dodge_cost             = "Dodge Cost";
inline constexpr auto dodge_key              = "Dodge Key";
inline constexpr auto use_sprint_key         = "Use Sprint Key";
inline constexpr auto use_mco_recover_window = "Only Dodge when MCO Recover Window is Open";

inline constexpr auto use_perk_lock       = "Lock Dodge behind Perk";
inline constexpr auto use_percentage_cost = "Dodge Cost is Percentage";

inline constexpr auto use_double_tap = "Double tab Dodge";

inline constexpr auto perk_mod_name  = "Mod Name for Dodge Perks";
inline constexpr auto spell_mod_name = "Mod Name for OnDodge Spell";

inline constexpr auto dodge_perk_form_ID      = "Dodge Perk Form ID";
inline constexpr auto on_dodge_spell_form_ID  = "OnDodge Spell Form ID";
inline constexpr auto spell_lock_perk_form_ID = "OnDodge Spell Lock Perk Form ID";
inline constexpr auto remove_forward          = "Remove Forward Dodge";
inline constexpr auto disable_in_third        = "Disable in 3rd";
} // namespace LAB
namespace HEL
{
inline constexpr auto enable_sneak_key_dodge     = "Toggle Doding with Sneak Key";
inline constexpr auto enable_dodge_in_place      = "Toggle Dodge in Place";
inline constexpr auto step_dodge                 = "Toggle Step Dodge";
inline constexpr auto enable_sneak_dodge         = "Toggle Dodge while Sneaking";
inline constexpr auto enable_dodge_attack_cancel = "Toggle Dodge to cancel Attacks";
inline constexpr auto only_cancel_light          = "Cancel only light attacks if canceling is enabled";
inline constexpr auto i_frame_duration           = "Duration of invincibilty frames";
inline constexpr auto default_dodge_event        = "Set the default dodge Event";
inline constexpr auto sprinting_press_duration = "Minimum duration of sprint key press to not dodge and sprint instead";
inline constexpr auto sneaking_press_duration  = "Minimkum duration of sneak key press to not dodge and sneak instead";
inline constexpr auto use_double_tap           = "Double Tab Dodge/Sneak/Sprint key to dodge";

inline constexpr auto dodge_cost             = "Stamina const for dodging";
inline constexpr auto dodge_key              = "Key to dodge";
inline constexpr auto use_sprint_key         = "Toggle Dodge with Sprint Key";
inline constexpr auto use_mco_recover_window = "Toggle Dodge only when MCO Recover Window is Open";

inline constexpr auto use_perk_lock = "Toggle Dodging requires a Perk";
inline constexpr auto use_percentage_cost =
    "Stamina cost for Dodging uses a percentage of max Stamina instead of a flat value";

inline constexpr auto perk_mod_name  = "Name of th mod for Dodge Perks";
inline constexpr auto spell_mod_name = "Name of the mod for OnDodge Spell";

inline constexpr auto dodge_perk_form_ID      = "FormID for dodge perk. Needs to be in this format: 0x800";
inline constexpr auto on_dodge_spell_form_ID  = "FormID for OnDodge Spell, needs to be in this format: 0x800";
inline constexpr auto spell_lock_perk_form_ID = "FormID for Spell Lock Perk, needs to be in this format: 0x800";

inline constexpr auto remove_forward   = "Block forward dodging";
inline constexpr auto disable_in_third = "Disable in 3rd person";

} // namespace HEL
namespace VAR
{

inline bool enable_sneak_key_dodge;
inline bool enable_dodge_in_place;
inline bool step_dodge;
inline bool enable_sneak_dodge;
inline bool enable_dodge_attack_cancel;
inline bool only_cancel_light;
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


inline bool use_double_tap;
inline bool remove_forward;
inline bool disable_in_third;

} // namespace VAR
} // namespace MENU

} // namespace Dodge