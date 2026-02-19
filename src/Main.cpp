#include "InputEvents.h"
#include "Settings.h"
#include "ui.h"
#include "AnimationEvents.h"

bool showed = false;
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
    case SKSE::MessagingInterface::kPostLoadGame:
        if (!showed)
        {
            if (MiscUtil::ShowWarningForCrashGuard("TK Dodge Addon"))
            {
                REX::INFO("Crash Guard is loaded, show unsupported warning");
            };
            showed = true;
        }
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
