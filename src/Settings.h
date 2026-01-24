#pragma once
#include "mod-data.h"

namespace Config
{
struct Settings : public REX::Singleton<Settings>, MOD
{
    inline static REX::TOML::Bool enable_sneak_key_dodge{SECTION_SETTINGS, "bEndableSneakKeyDodge", false};
    inline static REX::TOML::Bool enable_dodge_in_place{SECTION_SETTINGS, "bEnableDodgeInPlace", false};
    inline static REX::TOML::Bool step_dodge{SECTION_SETTINGS, "bStepDodge", false};
    inline static REX::TOML::Bool enable_sneak_dodge{SECTION_SETTINGS, "bEnableSneakDodge", false};
    inline static REX::TOML::Bool enable_dodge_attack_cancel{SECTION_SETTINGS, "bEnableDodgeAttackCancel", true};
    inline static REX::TOML::F32 i_frame_duration{SECTION_SETTINGS, "fIFrameDuration", 0.3f};
    inline static REX::TOML::Str default_dodge_event{SECTION_SETTINGS, "sDefaultDodgeEvent",
                                                     std::string("TKDodgeBack")};
    inline static REX::TOML::F32 sprinting_press_duration{SECTION_SETTINGS, "fSprintingPressDuration", 0.3f};
    inline static REX::TOML::F32 sneaking_press_duration{SECTION_SETTINGS, "fSneakingPressDuration", 0.3f};

    inline static REX::TOML::F32 dodge_cost{SECTION_SETTINGS, "fDodgeCost", 15.0f};
    inline static REX::TOML::U32 dodge_key{SECTION_SETTINGS, "uDodgeKey", std::uint32_t(274)};
    inline static REX::TOML::Bool use_sprint_key{SECTION_SETTINGS, "bUseSprintKey", false};
    inline static REX::TOML::Bool use_mco_recover_window{SECTION_SETTINGS, "bUseMCORecoverWindow", false};

    inline static REX::TOML::Bool use_perk_lock{SECTION_SETTINGS, "bUsePerkLock", false};
    inline static REX::TOML::F32 crit_dodge_mult{SECTION_SETTINGS, "fCritDodgeMult", 0.66f};
    inline static REX::TOML::Bool use_crit_dodge{SECTION_SETTINGS, "bUseCritDodge", false};
    inline static REX::TOML::Bool use_percentage_cost{SECTION_SETTINGS, "bUsePercentageCost", false};

    inline static REX::TOML::Str perk_mod_name{SECTION_FORMS, "sPerkModName", std::string("")};
    inline static REX::TOML::Str spell_mod_name{SECTION_FORMS, "sSpellModName", std::string("")};

    inline static REX::TOML::U32 dodge_perk_form_ID{SECTION_FORMS, "uDodgePerkFormID", uint32_t{0x0}};
    inline static REX::TOML::U32 on_dodge_spell_form_ID{SECTION_FORMS, "uOnDodgeSpellFormID", uint32_t{0x0}};
    inline static REX::TOML::U32 spell_lock_perk_form_ID{SECTION_FORMS, "uSpellLockPerkFormID", uint32_t{0x0}};

    void UpdateSettings(bool a_save) noexcept
    {
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(TOML_PATH_DEFAULT.data(), TOML_PATH_CUSTOM.data());
        if (!a_save)
            toml->Load();

        else
            toml->Save();
    }
};
struct Forms : REX::Singleton<Forms>, MOD
{

    inline static RE::SpellItem *dummyDodgeSpell;
    inline static RE::BGSPerk *DodgePerkDummy;
    inline static RE::BGSPerk *dummySpellLockPerk;

    inline static RE::SpellItem *onDodgeSpell;
    inline static RE::BGSPerk *SpellLockPerk;
    inline static RE::BGSPerk *ActualDodgePerk;

    inline static RE::TESGlobal* TDMGlobal;

    static constexpr std::array slMenuNames{
        RE::BarterMenu::MENU_NAME,    RE::BookMenu::MENU_NAME,     RE::Console::MENU_NAME,
        RE::ContainerMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME, RE::DialogueMenu::MENU_NAME,
        RE::FavoritesMenu::MENU_NAME, RE::GiftMenu::MENU_NAME,     RE::InventoryMenu::MENU_NAME,
        RE::JournalMenu::MENU_NAME,   RE::LevelUpMenu::MENU_NAME,  RE::LockpickingMenu::MENU_NAME,
        RE::MagicMenu::MENU_NAME,     RE::MapMenu::MENU_NAME,      RE::RaceSexMenu::MENU_NAME,
        RE::SleepWaitMenu::MENU_NAME, RE::StatsMenu::MENU_NAME,    RE::TrainingMenu::MENU_NAME,
        RE::TutorialMenu::MENU_NAME,  RE::TweenMenu::MENU_NAME,
    };
    std::vector<std::string> MenuNames{ slMenuNames.begin(), slMenuNames.end() };
     
    void LoadForms() noexcept
    {
        RE::TESDataHandler *const dh = RE::TESDataHandler::GetSingleton();

        const auto mod_file = dh->LookupModByName(MOD_NAME);

        if (bool is_file_loaded = mod_file && mod_file->compileIndex != 0xFF; !is_file_loaded)
        {
            REX::FAIL("Can not load forms from {}, please enable the mod first", MOD_NAME);
            return;
        }

        dummyDodgeSpell = dh->LookupForm<RE::SpellItem>(DUMMY_DODGE_SPELL_FORMID, MOD_NAME);
        DodgePerkDummy = dh->LookupForm<RE::BGSPerk>(DODGE_PERK_DUMMY_FORMID, MOD_NAME);
        dummySpellLockPerk = dh->LookupForm<RE::BGSPerk>(DUMMY_SPELL_LOCK_PERK_FORMID, MOD_NAME);

        // Load Dynamic Forms
        if (Settings::dodge_perk_form_ID.GetValue() != 0x0 && Settings::perk_mod_name.GetValue() != "")
        {
            ActualDodgePerk = dh->LookupForm<RE::BGSPerk>(Settings::dodge_perk_form_ID.GetValue(),
                                                          Settings::perk_mod_name.GetValue());
            if (!ActualDodgePerk)
                REX::ERROR("Dodge perk lookup failed, please check your config file");
        }
        if (Settings::spell_lock_perk_form_ID.GetValue() != 0x0 && Settings::perk_mod_name.GetValue() != "")
        {
            SpellLockPerk = dh->LookupForm<RE::BGSPerk>(Settings::spell_lock_perk_form_ID.GetValue(),
                                                        Settings::perk_mod_name.GetValue());
            if (!SpellLockPerk)
                REX::ERROR("Spell Lock perk lookup failed, please check your config file");
        }
        if (Settings::on_dodge_spell_form_ID.GetValue() != 0x0 && Settings::spell_mod_name.GetValue() != "")
        {
            onDodgeSpell = dh->LookupForm<RE::SpellItem>(Settings::on_dodge_spell_form_ID.GetValue(),
                                                         Settings::spell_mod_name.GetValue());
            if (!onDodgeSpell)
                REX::ERROR("On Dodge spell lookup failed, please check your config file");
        }
        TDMGlobal = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
    };
};
} // namespace Config
