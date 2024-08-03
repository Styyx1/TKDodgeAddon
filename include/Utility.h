#pragma once
#include <Settings.h>
#include <Hooks.h>

#define PI  3.14159265f
#define PI8 0.39269908f

class Utility : public Singleton<Utility>
{
public:

    inline RE::NiPoint2 Vec2Rotate(const RE::NiPoint2& vec, float angle)
    {
        RE::NiPoint2 ret;
        ret.x = vec.x * cos(angle) - vec.y * sin(angle);
        ret.y = vec.x * sin(angle) + vec.y * cos(angle);
        return ret;
    }

    inline float Vec2Length(const RE::NiPoint2& vec) { return std::sqrtf(vec.x * vec.x + vec.y * vec.y); }

    inline RE::NiPoint2 Vec2Normalize(RE::NiPoint2& vec)
    {
        RE::NiPoint2 ret(0.f, 0.f);
        float        vecLength = Vec2Length(vec);
        if (vecLength == 0) {
            return ret;
        }
        float invlen = 1.0f / vecLength;
        ret.x        = vec.x * invlen;
        ret.y        = vec.y * invlen;
        return ret;
    }

    inline float DotProduct(RE::NiPoint2& a, RE::NiPoint2& b) { return a.x * b.x + a.y * b.y; }

    inline float CrossProduct(RE::NiPoint2& a, RE::NiPoint2& b) { return a.x * b.y - a.y * b.x; }

    inline float GetAngle(RE::NiPoint2& a, RE::NiPoint2& b) { return atan2(CrossProduct(a, b), DotProduct(a, b)); }

    inline bool isJumping(RE::Actor* a_actor)
    {
        bool result = false;
        return a_actor->GetGraphVariableBool("bInJumpState", result) && result;
    }

    inline bool NearbyEnemyAttacks(RE::Actor* a_act)
    {
        if (!a_act) {
            return false;
        }
        auto enemy = a_act->GetActorRuntimeData().currentCombatTarget;
        if (!a_act->GetActorRuntimeData().currentProcess->high && !enemy) {
            return false;
        }
        auto weap = enemy.get().get()->GetAttackingWeapon()->object->As<RE::TESObjectWEAP>();
        if (!weap) {
            return false;
        }
        if (enemy && enemy.get().get()->IsAttacking() && weap->IsWeapon() && weap->IsMelee()) {
            return true;
        } 
    }

    inline static bool ActorHasActiveMagicEffect(RE::Actor* a_actor ,RE::EffectSetting* a_effect)
    {
        auto               activeEffects = a_actor->AsMagicTarget()->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        for (auto& effect : *activeEffects) {
            setting = effect ? effect->GetBaseObject() : nullptr;
            if (setting) {
                if (setting == a_effect) {
                    return true;
                }
            }
        }
        return false;
    }


    inline bool CriticalDodge(RE::Actor* dodger) {
        const Settings* settings = Settings::GetSingleton();
        if (NearbyEnemyAttacks(dodger) && !ActorHasActiveMagicEffect(dodger, settings->CritDodgeCooldown)) {
            return true;
        }
        else
            return false;
    }

    inline bool PerkCheck(RE::Actor* a_actor)
    {
        DEBUG("start perk check");
        Settings* settings = Settings::GetSingleton();
        if (settings->perkLockBoolGlobal->value != 0) {
            DEBUG("use perk lock perk check");
            if (settings->ActualDodgePerk && settings->DodgePerkDummy == nullptr) {
                DEBUG("No perks selected. unlock dodging");
                return true;
            }
            if (settings->ActualDodgePerk == nullptr && a_actor->HasPerk(settings->DodgePerkDummy)) {
                DEBUG("no actual perk, but dummy perk = true");
                return true;
            }
            if (settings->ActualDodgePerk && !a_actor->HasPerk(settings->ActualDodgePerk)) {
                DEBUG("actual perk exists but player doesn't have it");
                return false;
            }

            if (settings->ActualDodgePerk && a_actor->HasPerk(settings->ActualDodgePerk)) {
                DEBUG("perk check success, allows dodging");
                return true;
            }
            else
                return false;
        }
        else
            return true;
    }

    inline bool isInCancelState(RE::Actor* a_actor)
    {
        const Settings* settings    = Settings::GetSingleton();

        if ((settings->enableDodgeAttackCancel && settings->useMCOrecoverWindow->value != 0) == true) {
            bool result       = false;
            bool recover      = false;
            DEBUG("test graph bool is {}", LogBool(a_actor->GetGraphVariableBool("IsAttackReady", result) && result || a_actor->GetGraphVariableBool("MCO_IsInRecovery", recover) && recover));
            if ((a_actor->GetGraphVariableBool("MCO_Transition", result) && result || a_actor->GetGraphVariableBool("MCO_IsInRecovery", recover) && recover) == true) {
                return true;
            }
            else
                return false;
        }
        if (settings->enableDodgeAttackCancel) {
            return true;
        }
        if (!settings->enableDodgeAttackCancel) {
            return false;
        }
    }

    inline std::string LogBool(bool bool_to_log)
    {
        if (bool_to_log) {
            return "true";
        }
        if (!bool_to_log) {
            return "false";
        }
    }

    void castOnDodgeSpell()
    {
        RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
        Settings*            settings = Settings::GetSingleton();
        RE::BGSPerk*         lockPerk;        
        if (!settings->SpellLockPerk) {
            lockPerk = settings->dummySpellLockPerk;
        }
        else
            lockPerk = settings->SpellLockPerk;
        RE::SpellItem* dodgeSpell;
        if (!settings->onDodgeSpell) {
            dodgeSpell = settings->dummyDodgeSpell;
        }
        else
            dodgeSpell = settings->onDodgeSpell;

        if (player && lockPerk && dodgeSpell && player->HasPerk(lockPerk)) {
            ApplySpell(player, player, dodgeSpell);
        }
    }

    bool GetDodgeEvent(std::string& a_event)
    {
        auto normalizedInputDirection = Vec2Normalize(RE::PlayerControls::GetSingleton()->data.prevMoveVec);
        if (normalizedInputDirection.x == 0.f && normalizedInputDirection.y == 0.f) {
            return false;
        }

        auto tdm_freeMov = RE::TESForm::LookupByEditorID<RE::TESGlobal>("TDM_DirectionalMovement");
        if (tdm_freeMov && tdm_freeMov->value) {
            DEBUG("TDM Free Movement, Force to Forward Dodge!");
            a_event = "TKDodgeForward";
        }
        else {
            RE::NiPoint2 forwardVector(0.f, 1.f);
            float        dodgeAngle = GetAngle(normalizedInputDirection, forwardVector);
            if (dodgeAngle >= -2 * PI8 && dodgeAngle < 2 * PI8) {
                a_event = "TKDodgeForward";
            }
            else if (dodgeAngle >= -6 * PI8 && dodgeAngle < -2 * PI8) {
                a_event = "TKDodgeLeft";
            }
            else if (dodgeAngle >= 6 * PI8 || dodgeAngle < -6 * PI8) {
                a_event = "TKDodgeBack";
            }
            else if (dodgeAngle >= 2 * PI8 && dodgeAngle < 6 * PI8) {
                a_event = "TKDodgeRight";
            }
        }

        return true;
    }

    // bunch of ugly checks
    inline bool canDodge(RE::PlayerCharacter* a_pc)
    {
        auto            playerControls    = RE::PlayerControls::GetSingleton();
        const Settings* settings          = Settings::GetSingleton();
        bool            bIsDodging        = false;
        auto            controlMap        = RE::ControlMap::GetSingleton();
        const auto      playerState       = a_pc->AsActorState();
        auto            attackState       = playerState->GetAttackState();

        DEBUG("calculated dodge cost for dodge check is {}", CalculatedDodgeCost(a_pc));

        return a_pc->GetGraphVariableBool("bIsDodging", bIsDodging) && !bIsDodging && ((attackState == RE::ATTACK_STATE_ENUM::kNone) || isInCancelState(a_pc))
               && (!playerState->IsSprinting() || !settings->useSprintKeyGlobal->value != 0) && (controlMap->IsMovementControlsEnabled() && controlMap->IsFightingControlsEnabled())
               && (!playerState->IsSneaking() || Settings::enableSneakDodge) && playerControls && playerControls->attackBlockHandler
               && playerControls->attackBlockHandler->inputEventHandlingEnabled && playerControls->movementHandler && playerControls->movementHandler->inputEventHandlingEnabled
               && (playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal && playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal
                   && playerState->GetFlyState() == RE::FLY_STATE::kNone) && PerkCheck(a_pc)
               && !playerState->IsSwimming() && !isJumping(a_pc) && !a_pc->IsInKillMove()
               && (a_pc->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) >= CalculatedDodgeCost(a_pc)) && PerkCheck(a_pc) && !a_pc->IsOverEncumbered();
    }

    void dodge()
    {
        const Settings* settings = Settings::GetSingleton();
        auto      pc       = RE::PlayerCharacter::GetSingleton();
        if (!canDodge(pc)) {
            DEBUG("cannot dodge");
            return;
        }
        DEBUG("dodging");
        std::string dodge_event = Settings::defaultDodgeEvent;
        if (!GetDodgeEvent(dodge_event) && !Settings::EnableDodgeInPlace)
            return;

        if (settings->stepDodge) {
            DEBUG("step dodge bool is {}", LogBool(settings->stepDodge));
            DEBUG("step dodge active");
            pc->SetGraphVariableInt("iStep", 2);
        }
        else
            pc->SetGraphVariableInt("iStep", 0);
        DEBUG("step dodge bool is {}", LogBool(settings->stepDodge));

        castOnDodgeSpell();
        pc->SetGraphVariableFloat("TKDR_IframeDuration", Settings::iFrameDuration); // Set invulnerable frame duration
        pc->NotifyAnimationGraph(dodge_event);                                      // Send TK Dodge Event
    }

    float CalculatedDodgeCost(RE::Actor* a_act)
    {
        const Settings* settings = Settings::GetSingleton();
        float           dodgeCostModifier = 1.0;
        std::string     usedAV            = "DodgeCostModifier";
        auto            cost_modifierAV   = Hooks::LookupActorValueByName(usedAV.c_str());
        auto            max_stam          = a_act->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kStamina);
        if (a_act->AsActorValueOwner()->GetActorValue(cost_modifierAV) != 0.0000) {
            dodgeCostModifier = a_act->AsActorValueOwner()->GetActorValue(cost_modifierAV);
            DEBUG("AVG installed, dodge cost modifier is {}", dodgeCostModifier);
        }

        if (settings->usePercentageCostGlobal->value != 0 )
            return ((max_stam / 100) * settings->DodgeCostGlobal->value) * dodgeCostModifier;
        else
            return settings->DodgeCostGlobal->value * dodgeCostModifier;
    }

    void applyDodgeCost()
    {
        auto pc = RE::PlayerCharacter::GetSingleton();
        if (pc && !pc->IsGodMode() /*&& !CriticalDodge(pc)*/) {            
            const Settings* settings          = Settings::GetSingleton();            
 
            pc->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, -CalculatedDodgeCost(pc)); 
            
        }
        /*if (pc && CriticalDodge(pc)) {
            const Settings* settings          = Settings::GetSingleton();
            float           dodgeCostModifier = GetDodgeCostModifier(settings->DodgeCostSpell);
            float           crit_dodge_modifier = settings->crit_dodge_mult;
            ApplySpell(pc, pc, settings->CriticalDodgeSpellDummy);
            pc->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, -settings->DodgeCostGlobal->value * dodgeCostModifier * crit_dodge_modifier);
        }*/
    }

    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType()) {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction: {
            return true;
        }
        default: {
            return false;
        }
        }
    }

    inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (IsPermanent(spell)) {
            target->AddSpell(spell);
        }
        else {
            caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
        }
    }

};
