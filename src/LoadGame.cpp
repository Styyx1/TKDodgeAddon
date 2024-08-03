#include "AnimationEvents.h"
#include "Hooks.h"
#include "InputEvents.h"
#include "MenuEventHandler.h"
#include "Settings.h"
#include "Utility.h"

namespace TKDodge
{
    void EventCallback(SKSE::MessagingInterface::Message* msg)
    {
        if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
            Settings* settings = Settings::GetSingleton();
            settings->LoadSettings();
            Input::InputEventSink::Register();
            animEventHandler::RegisterForPlayer();
            Hooks::install();
        }
    }
} // namespace TKDodge
