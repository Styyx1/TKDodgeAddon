#pragma once

struct MOD
{
	inline static constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/tk-dodge.toml";
	inline static constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/tk-dodge_custom.toml";
	inline static constexpr std::string_view SECTION_SETTINGS = "Settings";
	inline static constexpr std::string_view SECTION_FORMS = "Forms";

	inline static constexpr std::string_view MOD_NAME = "TKDodgeAddon.esp";

	inline static constexpr RE::FormID DUMMY_SPELL_LOCK_PERK_FORMID = 0x80F;
	inline static constexpr RE::FormID DUMMY_DODGE_SPELL_FORMID = 0x811;
	//maybe revisit the idea of that being globals later. Could do an in-between version to remove those
	inline static constexpr RE::FormID DODGE_COST_GLOBAL_FORMID = 0x803;
	inline static constexpr RE::FormID DODGE_KEY_GLOBAL_FORMID = 0x804;
	inline static constexpr RE::FormID USE_SPRINT_KEY_GLOBAL_FORMID = 0x805;
	inline static constexpr RE::FormID USE_MCO_RECOVER_WINDOW_GLOBAL_FORMID = 0x806;
	inline static constexpr RE::FormID PERK_LOCK_BOOL_GLOBAL_FORMID = 0x807;
	inline static constexpr RE::FormID USE_CRIT_DODGE_GLOBAL_FORMID = 0x808;
	inline static constexpr RE::FormID DODGE_PERK_DUMMY_FORMID = 0x809;
	inline static constexpr RE::FormID USE_PERCENTAGE_COST_GLOBAL_FORMID = 0x810;

	inline static constexpr std::string_view USED_AV = "DodgeCostModifier";
	inline static constexpr std::string_view EXTRA_DODGE_AV = "ExtraDodgeCostModifier";

	inline static constexpr std::string_view DODGE_COST_PERK = "ModDodgeCost";
	inline static constexpr std::string_view IFRAME_DURATION_PERK = "ModiFrameDuration";
};