#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"
#include "dodging.h"


namespace Hooks
{
    static bool bStoppingSprint = false;
    static bool bStopSneak = false;

    void SprintHandlerHook::ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
    {
        if (Config::Settings::use_sprint_key.GetValue()) {
            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
            const auto userEvent       = a_event->QUserEvent();

            if (const auto userEvents      = RE::UserEvents::GetSingleton(); userEvent == userEvents->sprint) {
                if (a_event->IsDown() && (playerCharacter->playerFlags.isSprinting)) { // stopping sprint
                    bStoppingSprint = true;
                }
                else if (a_event->HeldDuration() < Config::Settings::sprinting_press_duration.GetValue()) {
                    if (a_event->IsUp()) {
                        if (!Utility::IsInMenu())
                            Dodge::OnInput();
                        bStoppingSprint = false;
                    }
                    return;
                }
                else if (playerCharacter && (!playerCharacter->playerFlags.isSprinting && !bStoppingSprint)) {
                    a_event->heldDownSecs = 0.f;
                }
                else if (a_event->IsUp()) {
                    bStoppingSprint = false;
                }
            }
        }

        _sprintHandlerHook(a_this, a_event, a_data);
    }

    void SneakHandlerHook::ProcessButton(RE::SneakHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
    {
        if (Config::Settings::enable_sneak_key_dodge.GetValue()) {
            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
            const auto userEvent       = a_event->QUserEvent();

            if (const auto userEvents      = RE::UserEvents::GetSingleton(); userEvent == userEvents->sneak) {
                if (a_event->IsDown() && (playerCharacter->IsSneaking())) { // stopping sneak
                    bStopSneak = true;
                }
                else if (a_event->HeldDuration() < Config::Settings::sneaking_press_duration.GetValue()) {
                    if (a_event->IsUp()) {
                        if (!Utility::IsInMenu())
                            Dodge::OnInput();
                        bStopSneak = false;
                    }
                    return;
                }
                else if (playerCharacter && (!playerCharacter->IsSneaking() && !bStopSneak)) {
                    a_event->heldDownSecs = 0.f;
                }
                else if (a_event->IsUp()) {
                    bStopSneak= false;
                }
            }
        }
        _sneakHandlerHook(a_this, a_event, a_data);
    }


    bool AttackBuffer::IsBufferActive()
    {
        return GetSingleton()->active();
    }

    void AttackBuffer::ResetBuffer()
    {
        REX::DEBUG("clear "
                   "is called");
         return GetSingleton()->clear_if_expired();
    }

    void AttackBuffer::PushActive()
    {
        REX::DEBUG("Push active is called");
        GetSingleton()->push();
    }

    void AttackHandler::ProcessButton(RE::AttackBlockHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
    {
        REX::DEBUG("Inside attack handler");
        if (a_event->IsDown() || AttackBuffer::IsBufferActive())
        {
            AttackBuffer::PushActive();
            return _attackBlockHandlerHook(a_this, a_event, a_data);
        }
        AttackBuffer::ResetBuffer();

        _attackBlockHandlerHook(a_this, a_event, a_data);
    }

    void AttackBuffer::OnUpdate(RE::PlayerCharacter* a_this)
    {
        GetSingleton()->clear_if_expired();

        if (!GetSingleton()->active())
            return;

        // This is your "CanAttack" gate
        const bool accepted = a_this->NotifyAnimationGraph("attackStart");

        REX::DEBUG("ATTACK ATTEMPT → accepted: {}", accepted);

        GetSingleton()->consume();
    }

    void PlayerUpdateLoop::PlayerUpdate(RE::PlayerCharacter* a_this, float a_delta)
    {

        if (!a_this->IsAttacking())
            a_this->SetGraphVariableBool("DodgeCancelEnabled", true);

        AttackBuffer::OnUpdate(a_this);

        _playerUpdateLoopHook(a_this, a_delta);
    }


    static bool wasInMenu = false;
    void MainUpdateLoop::MainUpdate(float a_delta)
    {
        const bool inMenu = Utility::IsInMenu();

        const auto playerCharacter = RE::PlayerCharacter::GetSingleton();

        if (wasInMenu && !inMenu)
        {
            Dodge::g_menuBlocker.block();
            Dodge::ClearBuffer();
        }

        if (Dodge::g_menuBlocker.active())
        {
            Dodge::ClearBuffer();
        }
        else
            Dodge::Update(playerCharacter);

        wasInMenu = inMenu;

        _mainUpdateLoopHook(a_delta);
    }
} // namespace Hooks
