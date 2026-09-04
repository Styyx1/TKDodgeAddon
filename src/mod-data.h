#pragma once

namespace Dodge
{
inline constexpr auto TOML_PATH_DEFAULT = "Data/SKSE/Plugins/tk-dodge.toml";
inline constexpr auto TOML_PATH_CUSTOM  = "Data/SKSE/Plugins/tk-dodge_custom.toml";
inline constexpr auto SECTION_SETTINGS  = "Settings";
inline constexpr auto SECTION_FORMS     = "Forms";

inline constexpr auto MOD_NAME = "TKDodgeAddon.esp";

inline constexpr RE::FormID DUMMY_SPELL_LOCK_PERK_FORMID = 0x80F;
inline constexpr RE::FormID DUMMY_DODGE_SPELL_FORMID     = 0x811;
// maybe revisit the idea of that being globals later. Could do an in-between version to remove those
inline constexpr RE::FormID DODGE_COST_GLOBAL_FORMID             = 0x803;
inline constexpr RE::FormID DODGE_KEY_GLOBAL_FORMID              = 0x804;
inline constexpr RE::FormID USE_SPRINT_KEY_GLOBAL_FORMID         = 0x805;
inline constexpr RE::FormID USE_MCO_RECOVER_WINDOW_GLOBAL_FORMID = 0x806;
inline constexpr RE::FormID PERK_LOCK_BOOL_GLOBAL_FORMID         = 0x807;
inline constexpr RE::FormID USE_CRIT_DODGE_GLOBAL_FORMID         = 0x808;
inline constexpr RE::FormID DODGE_PERK_DUMMY_FORMID              = 0x809;
inline constexpr RE::FormID USE_PERCENTAGE_COST_GLOBAL_FORMID    = 0x810;

inline constexpr auto USED_AV        = "DodgeCostModifier";
inline constexpr auto EXTRA_DODGE_AV = "ExtraDodgeCostModifier";

inline constexpr auto DODGE_COST_PERK      = "ModDodgeCost";
inline constexpr auto IFRAME_DURATION_PERK = "ModiFrameDuration";
} // namespace Dodge
