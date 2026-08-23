#include "AnimationEvents.h"

#include "dodging.h"

namespace Dodge
{

EventResult AnimEventHandler::ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink,
                                              RE::BSAnimationGraphEvent* a_event,
                                              RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
    const auto player = RE::PlayerCharacter::GetSingleton();

    REX::INFO("inside hooked animhandler");

    if (a_event->tag == "TKDR_DodgeStart")
    {
        REX::INFO("Try to get player and apply cost to him");
        if (player)
            Dodge::ApplyDodgeCostActor(player);
    }

    if (a_event->tag == "MCO_AttackInitiate" || a_event->tag == "MCO_PowerAttackInitiate" ||
        a_event->tag == "MCO_attackEnterNotify")
    {
        if (player)
            player->SetGraphVariableBool("DodgeCancelEnabled", false);
    }

    if (a_event->tag == "MCO_WinOpen" || a_event->tag == "MCO_PowerWinOpen")
    {
        if (player)
            player->SetGraphVariableBool("DodgeCancelEnabled", true);
    }
    if (a_event->tag == "MCO_PowerWinClose" || a_event->tag == "MCO_WinClose")
    {
        if (player)
            player->SetGraphVariableBool("DodgeCancelEnabled", false);
    }
    return _process_PC(a_sink, a_event, a_eventSource);
};
} // namespace Dodge