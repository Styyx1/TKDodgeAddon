#pragma once
#include <Settings.h>

#define PI 3.14159265f
#define PI8 0.39269908f

namespace Utility
{

    inline static RE::NiPoint2 Vec2Rotate(const RE::NiPoint2 &vec, float angle)
    {
        RE::NiPoint2 ret;
        ret.x = vec.x * cos(angle) - vec.y * sin(angle);
        ret.y = vec.x * sin(angle) + vec.y * cos(angle);
        return ret;
    }

    inline static float Vec2Length(const RE::NiPoint2 &vec)
    {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
    }

    inline static RE::NiPoint2 Vec2Normalize(const RE::NiPoint2 &vec)
    {
        RE::NiPoint2 ret(0.f, 0.f);
        const float vecLength = Vec2Length(vec);
        if (vecLength == 0)
        {
            return ret;
        }
        const float invlen = 1.0f / vecLength;
        ret.x = vec.x * invlen;
        ret.y = vec.y * invlen;
        return ret;
    }

    inline static float DotProduct(const RE::NiPoint2 &a, const RE::NiPoint2 &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline static float CrossProduct(const RE::NiPoint2 &a, const RE::NiPoint2 &b)
    {
        return a.x * b.y - a.y * b.x;
    }

    inline static float GetAngle(const RE::NiPoint2 &a, const RE::NiPoint2 &b)
    {
        return atan2(CrossProduct(a, b), DotProduct(a, b));
    }

    inline static bool isJumping(const RE::Actor *a_actor)
    {
        bool result = false;
        return a_actor->GetGraphVariableBool("bInJumpState", result) && result;
    }

    inline static bool NearbyEnemyAttacks(const RE::Actor *a_act)
    {
        if (!a_act)
        {
            return false;
        }
        const auto enemy = a_act->currentCombatTarget;
        if (!a_act->currentProcess->high && !enemy)
        {
            return false;
        }
        const auto enemy_actor = enemy.get().get();
        if (!enemy_actor)
            return false;

        const auto weap = ActorUtil::getWieldingWeapon(enemy_actor);
        if (!weap)
        {
            return false;
        }
        if (enemy && enemy_actor->IsAttacking() && weap->IsWeapon() && weap->IsMelee())
        {
            return true;
        }
        return false;
    }

    inline static bool PerkCheck(const RE::Actor *a_actor)
    {
        using namespace Config;
        if (!Settings::use_perk_lock.GetValue())
        {
            return true;
        }

        RE::BGSPerk *perk{};
        if (Forms::ActualDodgePerk)
        {
            perk = Forms::ActualDodgePerk;
        }
        if (!perk)
        {
            perk = Forms::DodgePerkDummy;
        }

        return perk && a_actor->HasPerk(perk);
    }

    inline static bool isInCancelState(const RE::Actor *a_actor)
    {
        if (!a_actor)
            return false;
        if (!Config::Settings::enable_dodge_attack_cancel.GetValue())
            return false;
        if (!Config::Settings::use_mco_recover_window.GetValue())
            return true;
        bool transition = false;
        int recovery = 0;
        bool attackReady = false;
        bool hasTransition = a_actor->GetGraphVariableBool("MCO_Transition", transition) && transition;
        int hasRecovery = a_actor->GetGraphVariableInt("MCO_IsInRecovery", recovery) && recovery;
        a_actor->GetGraphVariableBool("IsAttackReady", attackReady);

        REX::DEBUG("hasTransition: {}, hasRecovery: {}, attackReady: {}", hasTransition, hasRecovery, attackReady);

        return hasTransition || hasRecovery || attackReady;
    }

    static inline void castOnDodgeSpell()
    {
        RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
        RE::BGSPerk *lockPerk;

        if (!Config::Forms::SpellLockPerk)
        {
            lockPerk = Config::Forms::dummySpellLockPerk;
        }
        else
            lockPerk = Config::Forms::SpellLockPerk;

        RE::SpellItem *dodgeSpell;
        if (!Config::Forms::onDodgeSpell)
        {
            dodgeSpell = Config::Forms::dummyDodgeSpell;
        }
        else
            dodgeSpell = Config::Forms::onDodgeSpell;

        if (player && lockPerk && dodgeSpell && player->HasPerk(lockPerk))
        {
            MagicUtil::ApplySpell(player, player, dodgeSpell);
        }
    }

    static inline bool GetDodgeEvent(std::string &a_event)
    {
        const auto normalizedInputDirection = Vec2Normalize(RE::PlayerControls::GetSingleton()->data.prevMoveVec);
        if (normalizedInputDirection.x == 0.f && normalizedInputDirection.y == 0.f)
        {
            return false;
        }

        if (Config::Forms::TDMGlobal && Config::Forms::TDMGlobal->Is(RE::FormType::Global) &&
            Config::Forms::TDMGlobal->value != 0)
        {
            REX::DEBUG("TDM Free Movement, Force to Forward Dodge!");
            a_event = "TKDodgeForward";
        }
        else
        {
            constexpr RE::NiPoint2 forwardVector(0.f, 1.f);
            if (const float dodgeAngle = GetAngle(normalizedInputDirection, forwardVector); dodgeAngle >= -2 * PI8 && dodgeAngle < 2 * PI8)
            {
                a_event = "TKDodgeForward";
            }
            else if (dodgeAngle >= -6 * PI8 && dodgeAngle < -2 * PI8)
            {
                a_event = "TKDodgeLeft";
            }
            else if (dodgeAngle >= 6 * PI8 || dodgeAngle < -6 * PI8)
            {
                a_event = "TKDodgeBack";
            }
            else if (dodgeAngle >= 2 * PI8 && dodgeAngle < 6 * PI8)
            {
                a_event = "TKDodgeRight";
            }
        }

        return true;
    }

    inline static bool IsInMenu()
    {
        return MiscUtil::IsAnyOfMenuOpen(RE::UI::GetSingleton(), Config::Forms::GetSingleton()->MenuNames);
    }

    static inline float CalculatedDodgeCost(const RE::Actor *a_act)
    {

        float dodgeCostModifier = 1.0;
        float extraDodgeCostMod = 1.0;
        const std::string usedAV = "DodgeCostModifier";
        const std::string extraDodgeAV = "ExtraDodgeCostModifier";
        const auto cost_modifierAV =  AVUtil::LookupActorValueByName(MOD::USED_AV.data());
        const auto max_stam = a_act->GetBaseActorValue(RE::ActorValue::kStamina);

        const auto stag_cost_mod_AV = AVUtil::LookupActorValueByName(MOD::EXTRA_DODGE_AV.data());

        if (a_act->GetActorValue(cost_modifierAV) != 0.0000)
        {
            dodgeCostModifier = a_act->GetActorValue(cost_modifierAV);
            REX::DEBUG("AVG installed, dodge cost modifier is {}", dodgeCostModifier);
        }
        if (a_act->GetActorValue(stag_cost_mod_AV) != 0.0000)
        {
            extraDodgeCostMod = a_act->GetActorValue(stag_cost_mod_AV);
            REX::DEBUG("AVG installed, extra dodge cost modifier is {}", extraDodgeCostMod);
        }
        float dodge_cost = Config::Settings::dodge_cost.GetValue();
        if (Config::Settings::use_percentage_cost.GetValue())
        {

            REX::DEBUG("Calculated Dodge Cost is {}",
                       ((max_stam / 100) * dodge_cost) * dodgeCostModifier * extraDodgeCostMod);

            return ((max_stam / 100) * dodge_cost) * dodgeCostModifier * extraDodgeCostMod;
        }

        else
        {
            REX::DEBUG("Calculated Dodge Cost is {}",
                dodge_cost * dodgeCostModifier * extraDodgeCostMod);
            return dodge_cost * dodgeCostModifier * extraDodgeCostMod;
        }
    }

    // bunch of ugly checks
    inline static bool canDodge(const RE::PlayerCharacter *a_pc)
    {
        auto playerControls = RE::PlayerControls::GetSingleton();

        bool bIsDodging = false;
        const auto controlMap = RE::ControlMap::GetSingleton();
        const auto attackState = a_pc->GetAttackState();

        return a_pc->GetGraphVariableBool("bIsDodging", bIsDodging) && !bIsDodging &&
               ((attackState == RE::ATTACK_STATE_ENUM::kNone) || isInCancelState(a_pc)) &&
               (!a_pc->IsSprinting() || !Config::Settings::use_sprint_key.GetValue()) &&
               (controlMap->IsMovementControlsEnabled() && controlMap->IsFightingControlsEnabled()) &&
               (!a_pc->IsSneaking() || Config::Settings::enable_sneak_dodge.GetValue()) && playerControls &&
               playerControls->attackBlockHandler && playerControls->attackBlockHandler->inputEventHandlingEnabled &&
               playerControls->movementHandler && playerControls->movementHandler->inputEventHandlingEnabled &&
               (a_pc->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                a_pc->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                a_pc->GetFlyState() == RE::FLY_STATE::kNone) &&
               PerkCheck(a_pc) && !a_pc->IsSwimming() && !isJumping(a_pc) && !a_pc->IsInKillMove() &&
               (a_pc->GetActorValue(RE::ActorValue::kStamina) >= CalculatedDodgeCost(a_pc)) && PerkCheck(a_pc) &&
               !a_pc->IsOverEncumbered() && !IsInMenu();
    }

    static inline void dodge()
    {
        auto pc = RE::PlayerCharacter::GetSingleton();
        if (!canDodge(pc))
        {
            REX::DEBUG("cannot dodge");
            return;
        }

        std::string dodge_event = Config::Settings::default_dodge_event.GetValue();
        if (!GetDodgeEvent(dodge_event) && !Config::Settings::enable_dodge_in_place.GetValue())
            return;
        REX::DEBUG("dodging");
        if (Config::Settings::step_dodge.GetValue())
        {
            REX::DEBUG("step dodge bool is {}", Config::Settings::step_dodge.GetValue());
            REX::DEBUG("step dodge active");
            pc->SetGraphVariableInt("iStep", 2);
        }
        else
            pc->SetGraphVariableInt("iStep", 0);
        REX::DEBUG("step dodge bool is {}", Config::Settings::step_dodge.GetValue());

        castOnDodgeSpell();
        pc->SetGraphVariableFloat("TKDR_IframeDuration",
                                  Config::Settings::i_frame_duration.GetValue()); // Set invulnerable frame duration
        pc->NotifyAnimationGraph(dodge_event);                                    // Send TK Dodge Event
    }



    static inline void applyDodgeCost()
    {
        if (const auto pc = RE::PlayerCharacter::GetSingleton(); pc && !RE::PlayerCharacter::IsGodMode())
        {
            pc->DamageActorValue(RE::ActorValue::kStamina, -CalculatedDodgeCost(pc));
        }
    }
};
