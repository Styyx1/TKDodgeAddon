#pragma once

namespace Hooks
{
    struct SprintHandlerHook
    {      

    private:
        static void ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
        inline static REL::HookVFT _sprintHandlerHook{ RE::VTABLE_SprintHandler[0], 0x4, ProcessButton };
    };

    struct SneakHandlerHook
    {
    private:
        static void ProcessButton(RE::SneakHandler* a_this, RE::ButtonEvent* a_event,RE::PlayerControlsData* a_data);
        inline static REL::HookVFT _sneakHandlerHook{ RE::VTABLE_SneakHandler[0], 0x4, ProcessButton };
    };

    struct PlayerUpdateLoop
    {
    private:
        static void PlayerUpdate(RE::PlayerCharacter* a_this, float a_delta);
        inline static REL::HookVFT _playerUpdateLoopHook{ RE::VTABLE_PlayerCharacter[0], 0xAD, PlayerUpdate };
    };

    struct MainUpdateLoop
    {
        static void MainUpdate(float a_delta);
        inline static REL::Hook _mainUpdateLoopHook{ REL::ID(36564), 0xc26, MainUpdate };
    };



} // namespace Hooks
