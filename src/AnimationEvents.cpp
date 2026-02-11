#include "AnimationEvents.h"
#include "dodging.h"

constexpr uint32_t hash_djb2(const char* data, const size_t size) noexcept
{
    uint32_t hash = 5381;

    for (const char* c = data; c < data + size; ++c) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(*c);
    }

    return hash;
}

constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
{
    return hash_djb2(str, size);
}
constexpr uint32_t hash(const char* data, const size_t size) noexcept
{
    uint32_t hash = 5381;

    for (const char* c = data; c < data + size; ++c) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(*c);
    }

    return hash;
}

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src)
{
    const FnProcessEvent fn = fnHash.at(*reinterpret_cast<uint64_t*>(this));

    REX::DEBUG(" Event {} recieved", a_event.tag.data());
    REX::DEBUG(" Payload {} recieved", a_event.payload.c_str());

    auto* actor = const_cast<RE::Actor*>(a_event.holder->As<RE::Actor>());

    if (!actor)
    {
        return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
    }

    if (a_event.tag == "TKDR_DodgeStart") {
        Dodge::ApplyDodgeCostActor(actor);
    }

    if (a_event.tag == "MCO_AttackInitiate" ||a_event.tag == "MCO_PowerAttackInitiate" || a_event.tag == "MCO_attackEnterNotify")
    {
        actor->SetGraphVariableBool("DodgeCancelEnabled", false);
    }

    if (a_event.tag == "MCO_WinOpen" || a_event.tag == "MCO_PowerWinOpen")
    {
        actor->SetGraphVariableBool("DodgeCancelEnabled", true);
    }
    if (a_event.tag == "MCO_PowerWinClose" || a_event.tag == "MCO_WinClose")
    {
        actor->SetGraphVariableBool("DodgeCancelEnabled", false);
    }


    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;
