#include "dodging.h"
#include "Utility.h"
#include "Settings.h"
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
                buffer.push();
            }
        }
        else
        {
            buffer.push();
        }
    };

    void Update(RE::Actor* a_actor)
    {
        if (!buffer.consume())
            return;

        if (CanDodge(a_actor))
        {
            DoDodge(a_actor);
        }
        else
        {
            buffer.push();
        }

    }
    bool CanDodge(RE::Actor* a_actor)
    {
        auto playerControls = RE::PlayerControls::GetSingleton();

        bool bIsDodging = false;
        const auto controlMap = RE::ControlMap::GetSingleton();
        const auto attackState = a_actor->GetAttackState();

        return a_actor->GetGraphVariableBool("bIsDodging", bIsDodging) && !bIsDodging &&
               ((attackState == RE::ATTACK_STATE_ENUM::kNone) || Utility::isInCancelState(a_actor)) &&
               (!a_actor->IsSprinting() || !Config::Settings::use_sprint_key.GetValue()) &&
               (controlMap->IsMovementControlsEnabled() && controlMap->IsFightingControlsEnabled()) &&
               (!a_actor->IsSneaking() || Config::Settings::enable_sneak_dodge.GetValue()) && playerControls &&
               playerControls->attackBlockHandler && playerControls->attackBlockHandler->inputEventHandlingEnabled &&
               playerControls->movementHandler && playerControls->movementHandler->inputEventHandlingEnabled &&
               (a_actor->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                a_actor->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                a_actor->GetFlyState() == RE::FLY_STATE::kNone) &&
               Utility::PerkCheck(a_actor) && !a_actor->IsSwimming() && !Utility::isJumping(a_actor) && !a_actor->IsInKillMove() &&
               (a_actor->GetActorValue(RE::ActorValue::kStamina) >= Utility::CalculatedDodgeCost(a_actor)) && Utility::PerkCheck(a_actor) &&
               !a_actor->IsOverEncumbered() && !Utility::IsInMenu();
    }

    void DoDodge(RE::Actor* a_actor)
    {
        if (!CanDodge(a_actor))
        {
            REX::DEBUG("cannot dodge");
            return;
        }

        std::string dodge_event = Config::Settings::default_dodge_event.GetValue();
        if (!Utility::GetDodgeEvent(dodge_event) && !Config::Settings::enable_dodge_in_place.GetValue())
            return;
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

        Utility::castOnDodgeSpell();
        a_actor->SetGraphVariableFloat("TKDR_IframeDuration",
                                  Config::Settings::i_frame_duration.GetValue()); // Set invulnerable frame duration
        a_actor->NotifyAnimationGraph(dodge_event);
    }
}
