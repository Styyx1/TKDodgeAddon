#include "AnimationEvents.h"
#include "InputEvents.h"
#include "Settings.h"
#include "menu/sksemenu.h"


bool showed = false;
void Listener(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
        case SKSE::MessagingInterface::kInputLoaded:
            REX::TSingleton<Dodge::InputEvent>::GetSingleton()->RegisterInput();
            break;
        case SKSE::MessagingInterface::kDataLoaded:
            Dodge::FORMS::LoadForms();
            Dodge::MENU::SKSEMENU::RegisterSKSEMenu();
            break;
        default:
            break;
    }
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* skse)
{
    Init(skse, {.trampoline = true});

    Dodge::CONF::UpdateSettings();
    /* Menu::RegisterDodgeMenu(); */
    if (const auto messaging{SKSE::GetMessagingInterface()}; !messaging->RegisterListener(Listener))
    {
        return false;
    }
    return true;
}
