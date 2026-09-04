#pragma once
#define NOMINMAX


namespace Dodge
{
    using EventResult = RE::BSEventNotifyControl;
    struct InputEvent : REX::TSingleton<InputEvent>, RE::BSTEventSink<RE::InputEvent*>
    {
        void RegisterInput();
        EventResult ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>*) override;
    };
}
