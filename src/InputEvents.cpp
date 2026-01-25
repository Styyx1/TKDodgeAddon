#include "InputEvents.h"
#include "Settings.h"
#include "ui.h"
#include "dodging.h"


namespace Events
{
    void InputEvent::RegisterInput(){
        if (const auto manager = RE::BSInputDeviceManager::GetSingleton()) {
            manager->AddEventSink(this);
            REX::INFO("Registered for {}", typeid(RE::InputEvent).name());
        }
        else {
            REX::ERROR("Failed to register input event.");
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
                const auto button = e->AsButtonEvent();
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
            return EventResult::kContinue;
        }
        else
        {
            for (auto e = *a_event; e; e = e->next)
            {
                const auto button = e->AsButtonEvent();
                if (!button || !button->IsDown())
                    continue;

                uint32_t id = button->GetIDCode();

                switch (button->GetDevice()) {
                case RE::INPUT_DEVICE::kMouse:
                    id += SKSE::InputMap::kMacro_MouseButtonOffset;
                    break;
                case RE::INPUT_DEVICE::kGamepad:
                    id = SKSE::InputMap::GamepadMaskToKeycode(id);
                    break;
                default:
                    break;
                }

                if (id != Config::Settings::dodge_key.GetValue())
                    continue;

                Dodge::OnInput();
            }
        }



        return EventResult::kContinue;
    }
}
