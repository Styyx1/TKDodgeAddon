#pragma once

namespace Hooks
{
    void install();


    static RE::ActorValue LookupActorValueByName(const char* av_name)
    {
        // SE: 0x3E1450, AE: 0x3FC5A0, VR: ---
        using func_t = decltype(&LookupActorValueByName);
        REL::Relocation<func_t> func{ REL::RelocationID(26570, 27203) };
        return func(av_name);
    }

    class SprintHandlerHook
    {
    public:
        static void Hook()
        {
            REL::Relocation<std::uintptr_t> SprintHandlerVtbl{ RE::VTABLE_SprintHandler[0] }; // 166F1D8
            _ProcessButton = SprintHandlerVtbl.write_vfunc(0x4, ProcessButton);
        }

        

    private:
        static void ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);

        inline static REL::Relocation<decltype(ProcessButton)> _ProcessButton;
    };

} // namespace Hooks
