#include "dodging.h"
#include "Utility.h"
#include "Settings.h"
#include "API/PerkEntryPointExtenderAPI.h"
#include "mod-data.h"

namespace Dodge
{
    static DodgeInputBuffer buffer;
    static DoublePress dodgeDoublePress;

    void OnInput()
    {
        if (Config::Settings::use_double_tap.GetValue())
        {
            if (dodgeDoublePress.on_key_pressed())
            {
                dodgeDoublePress.last_press = {};
                buffer.push();
            }
        }
        else
        {
            dodgeDoublePress.last_press = {};
            buffer.push();
        }
    };

    void Update(RE::Actor* a_actor)
    {
        if (!buffer.can_fire())
            return ;

        if (!CanDodge(a_actor))
            return ;


        if (DoDodge(a_actor))
        {
            buffer.mark_fired();
        };

    }
    bool CanDodge(RE::Actor* a_actor)
    {
        if (!a_actor) // should be impossible at this point but this makes it pretty much impossible for the coming checks to fail on nullptr
        {
            return false;
        }
        const auto result = GetDodgeResult(a_actor);
        if (result == DodgeResult::kSuccess)
        {
            return true;
        }
        REX::DEBUG("Dodge failed because of: {}", DodgeResultToString(result));

        return false;
    }

    DodgeResult PerkCheck(const RE::Actor* a_actor)
    {
        using namespace Config;
        if (!Settings::use_perk_lock.GetValue())
        {
            return DodgeResult::kSuccess;
        }

        if (IsInGodModeHelper(a_actor))
        {
            return DodgeResult::kSuccess;
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
        if (perk && a_actor->HasPerk(perk))
        {
            return DodgeResult::kSuccess;
        }

        return DodgeResult::kHasNoPerk;
    }

    DodgeResult IsDodging(const RE::Actor* a_actor)
    {
        if (bool bIsDodging = false; a_actor->GetGraphVariableBool("bIsDodging", bIsDodging) && !bIsDodging)
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsDodging;
    }

    bool IsInMCORecovery(const RE::Actor* a_actor)
    {
        if (!Config::Settings::use_mco_recover_window.GetValue())
            return true;

        bool enabled = false;
        const bool isCancelEnabled = a_actor->GetGraphVariableBool("DodgeCancelEnabled", enabled) && enabled;
        return isCancelEnabled;
    }

    float CalculateDodgeCost(RE::Actor* a_actor)
    {
        if (const bool is_player_god_mod = a_actor->IsPlayerRef() && RE::PlayerCharacter::IsGodMode(); is_player_god_mod)
        {
            return 0.0;
        }

        float dodgeCostModifier = 1.0;
        float extraDodgeCostMod = 1.0;

        const auto cost_modifierAV =  AVUtil::LookupActorValueByName(MOD::USED_AV.data());
        const auto stag_cost_mod_AV = AVUtil::LookupActorValueByName(MOD::EXTRA_DODGE_AV.data());

        if (cost_modifierAV != RE::ActorValue::kNone)
        {
            dodgeCostModifier = a_actor->GetActorValue(cost_modifierAV);
        }
        if (stag_cost_mod_AV != RE::ActorValue::kNone)
        {
            extraDodgeCostMod = a_actor->GetActorValue(stag_cost_mod_AV);
        }
        float dodge_cost = Config::Settings::dodge_cost.GetValue();
        float cost = dodge_cost;
        if (Config::Settings::use_percentage_cost.GetValue())
        {
            const auto max_stam = a_actor->GetBaseActorValue(RE::ActorValue::kStamina);
            cost = ((max_stam / 100) * dodge_cost) * dodgeCostModifier * extraDodgeCostMod;
        }
        else
        {
            cost = dodge_cost * dodgeCostModifier * extraDodgeCostMod;
        }
        RE::TESForm* armo = a_actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody, true);

        RE::HandleEntryPoint(RE::PerkEntryPoint::kModPowerAttackStamina, a_actor, cost, MOD::DODGE_COST_PERK, armo);
        REX::DEBUG("Dodge Cost after is {}", cost);

        return cost;
    }

    void ApplyDodgeCostActor(RE::Actor* a_actor)
    {
        if (a_actor->IsPlayerRef() && RE::PlayerCharacter::IsGodMode())
            return;

        const float dodgeCost = CalculateDodgeCost(a_actor);
        a_actor->DamageActorValue(RE::ActorValue::kStamina, -dodgeCost);
    }

    bool GetDodgeEvent(std::string &a_event)
    {
        const auto normalizedInputDirection = Utility::Vec2Normalize(RE::PlayerControls::GetSingleton()->data.prevMoveVec);
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
            if (const float dodgeAngle = Utility::GetAngle(normalizedInputDirection, forwardVector); dodgeAngle >= -2 * PI8 && dodgeAngle < 2 * PI8)
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

    void CastOnDodgeSpell(RE::Actor* a_actor)
    {
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

        if (a_actor && lockPerk && dodgeSpell && a_actor->HasPerk(lockPerk))
        {
            MagicUtil::ApplySpell(a_actor, a_actor, dodgeSpell);
        }
    }


    bool CanAttackCancel(const RE::Actor* a_actor)
    {
        const bool cancel_enabled = Config::Settings::enable_dodge_attack_cancel.GetValue();
        const bool only_light_cancel = Config::Settings::only_cancel_light.GetValue();

        if (cancel_enabled)
        {
            if (only_light_cancel)
            {
                return !a_actor->IsPowerAttacking();
            }
            return IsInMCORecovery(a_actor);
        }
        return false;
    }

    DodgeResult IsInAttackState(const RE::Actor* a_actor)
    {
        if (const auto attackState = a_actor->GetAttackState(); attackState == RE::ATTACK_STATE_ENUM::kNone || CanAttackCancel(a_actor))
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsAttacking;
    }

    DodgeResult IsSprinting(const RE::Actor* a_actor)
    {
        if (!a_actor->IsSprinting())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsSprinting;
    }

    DodgeResult IsSneaking(const RE::Actor* a_actor)
    {
        if (!a_actor->IsSneaking() || Config::Settings::enable_sneak_dodge.GetValue())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsSneaking;
    }
    DodgeResult IsSwimming(const RE::Actor* a_actor)
    {
        if (!a_actor->IsSwimming())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsSwimming;
    }

    DodgeResult IsInKillMove(const RE::Actor* a_actor)
    {
        if (!a_actor->IsInKillMove())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsInKillMove;
    }

    bool IsInGodModeHelper(const RE::Actor* a_actor)
    {
        if (a_actor->IsPlayerRef() && RE::PlayerCharacter::IsGodMode())
        {
            return true;
        }
        return false;
    }

    DodgeResult IsInMenu(const RE::Actor* a_actor)
    {
        if (!a_actor->IsPlayerRef())
        {
            return DodgeResult::kSuccess;
        }

        if (!Utility::IsInMenu())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsInMenu;
    }

    DodgeResult IsControlsDisabled(const RE::Actor* a_actor)
    {
        if (!a_actor->IsPlayerRef())
        {
            return DodgeResult::kSuccess;
        }

        if (const auto playerControls = RE::PlayerControls::GetSingleton(); playerControls &&
               playerControls->attackBlockHandler && playerControls->attackBlockHandler->inputEventHandlingEnabled &&
               playerControls->movementHandler && playerControls->movementHandler->inputEventHandlingEnabled)
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsControlsDisabled;
    }

    DodgeResult IsInWrongState(const RE::Actor* a_actor)
    {
        if (a_actor->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                a_actor->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                a_actor->GetFlyState() == RE::FLY_STATE::kNone)
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsInWrongState;
    }

    DodgeResult HasStamina( RE::Actor* a_actor)
    {
        if (IsInGodModeHelper(a_actor))
        {
            return DodgeResult::kSuccess;
        }
        if (a_actor->GetActorValue(RE::ActorValue::kStamina) >= CalculateDodgeCost(a_actor))
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kStamina;
    }

    DodgeResult IsOverencumbered(const RE::Actor* a_actor)
    {
        if (IsInGodModeHelper(a_actor))
        {
            return DodgeResult::kSuccess;
        }
        if (!a_actor->IsOverEncumbered())
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kOverencumbered;
    }

    DodgeResult IsJumping(const RE::Actor* a_actor)
    {
        if (!Utility::isJumping(a_actor))
        {
            return DodgeResult::kSuccess;
        }
        return DodgeResult::kIsJumping;
    }

    DodgeResult IsThirdPersonAllowed(const RE::Actor* a_actor)
    {
        if (Config::Settings::disable_in_third.GetValue())
        {
            if (a_actor->IsPlayerRef())
            {
                if (const auto playerCam = RE::PlayerCamera::GetSingleton(); playerCam && playerCam->IsInThirdPerson())
                {
                    return DodgeResult::kInThirdPerson;
                }
            }
        }
        return DodgeResult::kSuccess;
    }

    DodgeResult GetDodgeResult( RE::Actor* a_actor)
    {
        if (!a_actor)
            return DodgeResult::kUnknown;

        if (const auto res = IsInMenu(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsControlsDisabled(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsInKillMove(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsInWrongState(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsJumping(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsSwimming(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsDodging(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsInAttackState(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsSprinting(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsSneaking(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = HasStamina(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsOverencumbered(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = PerkCheck(a_actor); res != DodgeResult::kSuccess)
            return res;

        if (const auto res = IsThirdPersonAllowed(a_actor); res != DodgeResult::kSuccess)
            return res;

        return DodgeResult::kSuccess;
    }



    bool DoDodge(RE::Actor* a_actor)
    {
        if (!CanDodge(a_actor))
        {
            REX::DEBUG("cannot dodge");
            return false;
        }

        std::string dodge_event = Config::Settings::default_dodge_event.GetValue();
        if (!GetDodgeEvent(dodge_event) && !Config::Settings::enable_dodge_in_place.GetValue())
            return false;
        REX::DEBUG("dodging");
        if (Config::Settings::step_dodge.GetValue())
        {
            REX::DEBUG("step dodge bool is {}", Config::Settings::step_dodge.GetValue());
            REX::DEBUG("step dodge active");
            a_actor->SetGraphVariableInt("iStep", 2);
        }
        else
            a_actor->SetGraphVariableInt("iStep", 0);
        REX::DEBUG("step dodge bool is {}", Config::Settings::step_dodge.GetValue());

        CastOnDodgeSpell(a_actor);
        float iFrames = Config::Settings::i_frame_duration.GetValue();

        RE::TESForm* armo = a_actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody, true);
        RE::HandleEntryPoint(RE::PerkEntryPoint::kModPowerAttackStamina, a_actor, iFrames, MOD::IFRAME_DURATION_PERK, armo);

        iFrames = std::clamp(iFrames, 0.0f, 3.0f);

        a_actor->SetGraphVariableFloat("TKDR_IframeDuration", iFrames); // Set invulnerable frame duration
        return a_actor->NotifyAnimationGraph(dodge_event);
    }
}
