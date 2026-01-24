#pragma once

namespace Hooks
{
    struct SprintHandlerHook
    {      

    private:
        static void ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
        inline static REL::HookVFT _SprintHandlerHook{ RE::VTABLE_SprintHandler[0], 0x4, ProcessButton };
    };

} // namespace Hooks
