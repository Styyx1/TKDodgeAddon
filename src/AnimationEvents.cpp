#include "AnimationEvents.h"
#include "Utility.h"

constexpr uint32_t hash(const char* data, const size_t size) noexcept
{
    uint32_t hash = 5381;

    for (const char* c = data; c < data + size; ++c) {
        hash = ((hash << 5) + hash) + (unsigned char)*c;
    }

    return hash;
}

constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
{
    return hash(str, size);
}

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src)
{
    FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
    // DEBUG(" Event {} recieved", a_event.tag.data());

    // DEBUG(" Payload {} recieved", a_event.payload.c_str());
    // Settings* settings = Settings::GetSingleton();
    auto util = Utility::GetSingleton();

    if (a_event.tag == "TKDR_DodgeStart") {
        util->applyDodgeCost();
    }

    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;
