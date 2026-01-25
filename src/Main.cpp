#include "InputEvents.h"
#include "Settings.h"
#include "ui.h"
#include "AnimationEvents.h"

void Listener(SKSE::MessagingInterface::Message *a_msg)
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kInputLoaded:
        Events::InputEvent::GetSingleton()->RegisterInput();
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        Config::Settings::crit_dodge_mult.SetValue(1.f);
        Config::Settings::UpdateSettings(true);
        animEventHandler::RegisterForPlayer();
        break;

    default:
        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse)
{
    Init(skse, {.trampoline = true});

    Config::Settings::UpdateSettings(false);
    Menu::RegisterDodgeMenu();
    if (const auto messaging{SKSE::GetMessagingInterface()}; !messaging->RegisterListener(Listener))
    {
        return false;
    }
    return true;
}
