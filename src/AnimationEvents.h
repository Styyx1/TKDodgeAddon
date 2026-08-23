#pragma once

namespace Dodge
{

using EventResult = RE::BSEventNotifyControl;

struct AnimEventHandler
{

    static EventResult ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink,
                                       RE::BSAnimationGraphEvent* a_event,
                                       RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource);

    static inline REL::THookVFT _process_PC{RE::PlayerCharacter::VTABLE[2], 0x1, ProcessEvent_PC};
};

} // namespace Dodge