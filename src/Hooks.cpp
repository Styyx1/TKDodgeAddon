#include "Hooks.h"
#include "InputEvents.h"
#include "Settings.h"
#include "Utility.h"

namespace Hooks
{
    static bool bStoppingSprint = false;

    void SprintHandlerHook::ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
    {
        if (Config::Settings::use_sprint_key.GetValue()) {
            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
            const auto userEvent       = a_event->QUserEvent();

            if (const auto userEvents      = RE::UserEvents::GetSingleton(); userEvent == userEvents->sprint) {
                if (a_event->IsDown() && (playerCharacter->playerFlags.isSprinting)) { // stopping sprint
                    REX::DEBUG("key pressesd while pc is sprinting - stopping sprint");
                    bStoppingSprint = true;
                }
                else if (a_event->HeldDuration() < Config::Settings::sprinting_press_duration.GetValue()) { // TODO:ADD THIS to settings
                    if (a_event->IsUp()) {
                        REX::DEBUG("key lifted and was pressed less then SprintingPressDuration - dodging");
                        Utility* const util = Utility::GetSingleton();
                        util->dodge();
                        bStoppingSprint = false;
                    }
                    return;
                }
                else if (playerCharacter && (!playerCharacter->playerFlags.isSprinting && !bStoppingSprint)) {
                    REX::DEBUG("player not sprinting and StoopingSprint flag is false - ressetting heldDownSecs to 0");
                    a_event->heldDownSecs = 0.f;
                }
                else if (a_event->IsUp()) {
                    REX::DEBUG("key lifted - resuming sprint");
                    bStoppingSprint = false;
                }
            }
        }

        _SprintHandlerHook(a_this, a_event, a_data);
    }
} // namespace Hooks
