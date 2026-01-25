#include "Hooks.h"
#include "InputEvents.h"
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

    void PlayerUpdateLoop::PlayerUpdate(RE::PlayerCharacter* a_this, float a_delta)
    {
        Dodge::Update(a_this);
        _playerUpdateLoopHook(a_this, a_delta);
    }
} // namespace Hooks
