#pragma once
#define NOMINMAX


namespace Events
{
    using EventResult = RE::BSEventNotifyControl;
    struct InputEvent : REX::Singleton<InputEvent>, RE::BSTEventSink<RE::InputEvent*>
    {
        void RegisterInput();
        EventResult ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>*) override;
    };
}
