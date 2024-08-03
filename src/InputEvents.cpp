#include "InputEvents.h"
#include "AnimationEvents.h"
#include "Settings.h"
#include "Utility.h"
#include <CLIBUtil/Key.h>

namespace
{
    class HotkeyContext
    {
    public:
        explicit HotkeyContext(const Settings* settings) : hotkey(settings->DodgeKeyGlobal->value) {}

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed() && !a_button->IsHeld()) {
                auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey.Update(key);
            }
        }

        void Finalize(Input::InputEventSink* app)
        {
            for (std::uint32_t count = 2; count > 0; --count) {
                bool done = false;
                if (hotkey.IsActive()) {
                    Utility* util = Utility::GetSingleton();
                    logger::debug("key was pressed");
                    util->dodge();
                    logger::debug("dodged");
                    done = true;
                }
                if (done) {
                    break;
                }
            }
        }

    private:
        CLib::Key hotkey;
    };
} // namespace

void Input::HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    auto config = Settings::GetSingleton();

    HotkeyContext ctx{ config };

    for (auto event = *a_event; event; event = event->next) {
        if (auto button = event->AsButtonEvent()) {
            ctx.Update(button);
        }
    }
    auto app = Input::InputEventSink::GetSingleton();

    ctx.Finalize(app);
}
