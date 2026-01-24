#include "InputEvents.h"
#include "AnimationEvents.h"
#include "Settings.h"
#include "Utility.h"
#include "ui.h"


namespace Events{
    void InputEvent::RegisterInput(){
        if (auto manager = RE::BSInputDeviceManager::GetSingleton()) {
            manager->AddEventSink(this);
            REX::INFO("Registered for {}", typeid(RE::InputEvent).name());
        }
        else {
            REX::ERROR("Failed to register input event.");
        }
    }

    void InputEvent::RegisterHotkeys()
    {
        
    }

    void InputEvent::DoDodge(const hotkeys::KeyCombination* key)
    {
        if (key->IsTriggered()) {
            Utility* util = Utility::GetSingleton();
            util->dodge();
        }
    }

    EventResult InputEvent::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
    {
        if(!a_event){
            return EventResult::kContinue;
        }

        if (Menu::Settings::capture_key_input)
        {
            for (auto e = *a_event; e; e = e->next)
            {
                auto button = e->AsButtonEvent();
                if (!button || !button->HasIDCode())
                    continue;

                if (!button->IsDown())
                    continue;

                uint32_t key = button->GetIDCode();

                switch (button->GetDevice()) {
                case RE::INPUT_DEVICE::kMouse:
                    key += SKSE::InputMap::kMacro_MouseButtonOffset;
                    break;
                case RE::INPUT_DEVICE::kGamepad:
                    key = SKSE::InputMap::GamepadMaskToKeycode(key);
                    break;
                default:
                    break;
                }
                Menu::Settings::Var::dodge_key = key;
            }
        }



        dodge_action_key.Process(a_event); 

        return EventResult::kContinue;
    }

}
