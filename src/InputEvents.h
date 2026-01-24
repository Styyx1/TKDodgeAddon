#pragma once
#include "Settings.h"

using EventResult = RE::BSEventNotifyControl;

#define continueEvent RE::BSEventNotifyControl::kContinue

namespace Events{
    struct InputEvent : REX::Singleton<InputEvent>, RE::BSTEventSink<RE::InputEvent*>
    {
        void RegisterInput();
        void RegisterHotkeys();
        static void DoDodge(const hotkeys::KeyCombination* key);
        hotkeys::KeyCombination dodge_action_key{ DoDodge };
        EventResult ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
    };
}
