#include "Settings.h"

#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\TK Dodge RE.ini"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings...");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(SETTINGFILE_PATH);

    debug_logging = ini.GetBoolValue("Main", "EnableDebugLog");
    ReadBoolSetting(ini, "Main", "EnableSneakKeyDodge", EnableSneakKeyDodge);
    ReadBoolSetting(ini, "Main", "EnableDodgeInPlace", EnableDodgeInPlace);
    ReadBoolSetting(ini, "Main", "StepDodge", stepDodge);
    ReadBoolSetting(ini, "Main", "enableDodgeAttackCancel", enableDodgeAttackCancel);
    ReadBoolSetting(ini, "Main", "EnableSneakDodge", enableSneakDodge);
    ReadFloatSetting(ini, "Main", "iFrameDuration", iFrameDuration);
    defaultDodgeEvent = ini.GetValue("Main", "defaultDodgeEvent", "TKDodgeBack");
    ReadFloatSetting(ini, "Main", "SprintingPressDuration", SprintingPressDuration);
    //dodge perk. to actually dodge
    std::string perk_fileName(ini.GetValue("Perk", "sPerkModFileName", ""));
    std::string s_DodgeLockPerk_ID(ini.GetValue("Perk", "PerkLockFormID", ""));
    // spell lock perk. locks OnDodgeSpell behind perk
    std::string s_SpellLockPerk_ID(ini.GetValue("Perk", "SpellLockPerkFormID", ""));
    // OnDodgeSpell, fires whenever dodging is possible and key for it is pressed
    std::string spell_fileName(ini.GetValue("Spell", "sSpellModFileName", ""));
    std::string s_OnDodgeSpell_ID(ini.GetValue("Spell", "OnDodgeSpellFormID", ""));

    if (iFrameDuration < 0.f) {
        iFrameDuration = 0.f;
    }
    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }
    // Mod Name for perk locking
    if (!perk_fileName.empty()) {
        perkModName = perk_fileName;
        DEBUG("perk mod name is {}", perkModName);
        // Form ID from INI
        if (!s_DodgeLockPerk_ID.empty()) {
            DodgePerkFormID = ParseFormID(s_DodgeLockPerk_ID);
            DEBUG("found Dodge Perk FormID");
        }
        if (!s_SpellLockPerk_ID.empty()) {
            SpellLockPerkFormID = ParseFormID(s_SpellLockPerk_ID);
            DEBUG("found SpellLock Perk FormID");
        }
    }
    if (!spell_fileName.empty()) {
        spellModName = spell_fileName;
        DEBUG("spell mod name is {}", spellModName);
        // Form ID from INI
        if (!s_OnDodgeSpell_ID.empty()) {
            onDodgeSpellFormID = ParseFormID(s_OnDodgeSpell_ID);
            DEBUG("found OnDodgeSpell FormID");
        }
    }
    LoadGameData();
    logger::info("...finished loading settings \n");
}

void Settings::ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
    const char* bFound = nullptr;
    bFound             = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        INFO("found {} with value {}", a_settingName, bFound);
        a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
    }
}

void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
    const char* bFound = nullptr;
    bFound             = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        INFO("found {} with value {}", a_settingName, bFound);
        a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
    }
}

void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
    const char* bFound = nullptr;
    bFound             = a_ini.GetValue(a_sectionName, a_settingName);
    if (bFound) {
        INFO("found {} with value {}", a_settingName, bFound);
        a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
    }
}

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}

void Settings::LoadGameData() noexcept
{
    logger::info("loading forms...");

    auto        dataHandler = RE::TESDataHandler::GetSingleton();
    std::string FileName    = "TKDodgeAddon.esp";
    dummySpellLockPerk      = dataHandler->LookupForm(0x80F, FileName)->As<RE::BGSPerk>();
    DEBUG("SpellLock Perk found with name {}", dummySpellLockPerk->GetName());
    dummyDodgeSpell          = dataHandler->LookupForm(0x811, FileName)->As<RE::SpellItem>();
    DEBUG("Spell found with name {}", dummyDodgeSpell->GetName());
    DodgeCostGlobal = dataHandler->LookupForm(0x803, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", DodgeCostGlobal->GetFormEditorID(), DodgeCostGlobal->value);
    DodgeKeyGlobal = dataHandler->LookupForm(0x804, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", DodgeKeyGlobal->GetFormEditorID(), DodgeKeyGlobal->value);
    useSprintKeyGlobal = dataHandler->LookupForm(0x805, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", useSprintKeyGlobal->GetFormEditorID(), useSprintKeyGlobal->value);
    useMCOrecoverWindow = dataHandler->LookupForm(0x806, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", useMCOrecoverWindow->GetFormEditorID(), useMCOrecoverWindow->value);
    perkLockBoolGlobal = dataHandler->LookupForm(0x807, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", perkLockBoolGlobal->GetFormEditorID(), perkLockBoolGlobal->value);
    useCritDodgeGlobal = dataHandler->LookupForm(0x808, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", useCritDodgeGlobal->GetFormEditorID(), useCritDodgeGlobal->value);
    DodgePerkDummy = dataHandler->LookupForm(0x809, FileName)->As<RE::BGSPerk>();
    DEBUG("Dummy perk found. ID is {}", DodgePerkDummy->GetFormID());
    usePercentageCostGlobal = dataHandler->LookupForm(0x810, FileName)->As<RE::TESGlobal>();
    DEBUG("Global {} found, value is {}", usePercentageCostGlobal->GetFormEditorID(), usePercentageCostGlobal->value);

    // FormIDs from ini
    if (DodgePerkFormID) {
        ActualDodgePerk = skyrim_cast<RE::BGSPerk*>(dataHandler->LookupForm(DodgePerkFormID, perkModName));
        DEBUG("dodge perk lookup success");
    }
    if (SpellLockPerkFormID) {
        SpellLockPerk = skyrim_cast<RE::BGSPerk*>(dataHandler->LookupForm(SpellLockPerkFormID, perkModName));
        DEBUG("Spell Lock perk lookup success");
    }
    if (onDodgeSpellFormID) {
        onDodgeSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(onDodgeSpellFormID, spellModName));
        DEBUG("on dodge spell lookup success");
    }
    logger::info("...finished loading forms");
}
