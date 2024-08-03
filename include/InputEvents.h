#pragma once
#include "Settings.h"

using EventResult = RE::BSEventNotifyControl;

#define continueEvent RE::BSEventNotifyControl::kContinue

namespace Input
{
    class HotkeyManager
    {
    public:
        static void Process(const RE::InputEvent* const* a_event);
    };

    class InputEventSink final : public Singleton<InputEventSink>, public RE::BSTEventSink<RE::InputEvent*>
    {
        friend class Singleton<InputEventSink>;

    public:
        using Event       = RE::InputEvent*;
        using EventSource = RE::BSTEventSource<Event>;

        static void Register()
        {
            if (auto manager = RE::BSInputDeviceManager::GetSingleton()) {
                manager->AddEventSink(GetSingleton());
                SKSE::log::info("Successfully registered input event.");
            }
            else {
                SKSE::log::error("Failed to register input event.");
            }
        };

        RE::BSEventNotifyControl ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
        {
            if (!a_event || !RE::Main::GetSingleton()->gameActive) {
                return RE::BSEventNotifyControl::kContinue;
            }

            Input::HotkeyManager::Process(a_event);

            return RE::BSEventNotifyControl::kContinue;
        }

    private:
        InputEventSink() = default;
    };

} // namespace Input
