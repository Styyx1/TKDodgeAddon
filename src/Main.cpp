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
        animEventHandler::RegisterForPlayer();
        Config::Forms::LoadForms();
        break;
    case SKSE::MessagingInterface::kPostPostLoad:
        if (MiscUtil::ShowWarningForCrashGuard("TK Dodge Addon"))
        {
            REX::INFO("Crash Guard is loaded in post post load, show unsupported warning");
        };
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        if (MiscUtil::ShowWarningForCrashGuard("TK Dodge Addon"))
        {
            REX::INFO("Crash Guard is loaded, show unsupported warning");
        };
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
