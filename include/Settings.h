#pragma once

class Settings : public Singleton<Settings>
{
public:
    void LoadSettings() noexcept;
    void LoadGameData() noexcept;
    
    inline static bool          debug_logging{};
    // default TK Dodge Settings
    //inline static bool          EnableSprintKeyDodge    = false;
    inline static bool          EnableSneakKeyDodge     = false;
    inline static bool          EnableDodgeInPlace      = false;
    inline static bool          stepDodge               = false;
    inline static bool          enableSneakDodge        = false;
    inline static bool          enableDodgeAttackCancel = true;
    inline static float         iFrameDuration          = 0.3f;
    inline static std::string   defaultDodgeEvent       = "TKDodgeBack";
    inline static float         SprintingPressDuration  = 0.3f;
    inline static float         SneakingPressDuration   = 0.3f;
    // new TK Dodge Addon Settings

    inline static bool          usePerkLock;
    inline static float         crit_dodge_mult = 0.66f;
    inline static bool          useCritDodge;
    inline static bool          usePercentageCost;
    std::string                 perkModName;
    std::string                 spellModName;
    //Forms
    RE::FormID DodgePerkFormID;
    RE::FormID onDodgeSpellFormID;
    RE::FormID SpellLockPerkFormID;

    RE::SpellItem* dummyDodgeSpell;
    RE::SpellItem* onDodgeSpell;
    RE::SpellItem* CriticalDodgeSpellDummy;

    RE::TESGlobal* DodgeCostGlobal;
    RE::TESGlobal* DodgeKeyGlobal;
    RE::TESGlobal* useSprintKeyGlobal;
    RE::TESGlobal* useMCOrecoverWindow;
    RE::TESGlobal* perkLockBoolGlobal;
    RE::TESGlobal* useCritDodgeGlobal;
    RE::TESGlobal* usePercentageCostGlobal;
    
    RE::BGSPerk*       dummySpellLockPerk;
    RE::BGSPerk*       SpellLockPerk;
    RE::BGSPerk*   ActualDodgePerk;
    RE::BGSPerk*   DodgePerkDummy;

    RE::EffectSetting* CritDodgeCooldown;

    static constexpr std::array slMenuNames{
        RE::BarterMenu::MENU_NAME,
        RE::BookMenu::MENU_NAME,
        RE::Console::MENU_NAME,
        RE::ContainerMenu::MENU_NAME,
        RE::CraftingMenu::MENU_NAME,
        RE::DialogueMenu::MENU_NAME,
        RE::FavoritesMenu::MENU_NAME,
        RE::GiftMenu::MENU_NAME,
        RE::InventoryMenu::MENU_NAME,
        RE::JournalMenu::MENU_NAME,
        RE::LevelUpMenu::MENU_NAME,
        RE::LockpickingMenu::MENU_NAME,
        RE::MagicMenu::MENU_NAME,
        RE::MapMenu::MENU_NAME,
        RE::RaceSexMenu::MENU_NAME,
        RE::SleepWaitMenu::MENU_NAME,
        RE::StatsMenu::MENU_NAME,
        RE::TrainingMenu::MENU_NAME,
        RE::TutorialMenu::MENU_NAME,
        RE::TweenMenu::MENU_NAME,
    };
    std::vector<std::string> sl1MenuNames{ slMenuNames.begin(), slMenuNames.end() };
    
    static RE::FormID ParseFormID(const std::string& str);
    

private:
    static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
    static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
    static void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
};
