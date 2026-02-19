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

    struct AttackBuffer : REX::Singleton<AttackBuffer>
    {
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;


        static bool IsBufferActive();
        static void ResetBuffer();
        static void PushActive();
        static void OnUpdate(RE::PlayerCharacter* a_this);

    private:
        bool m_buffered = false;
        TimePoint queuedAt{Clock::duration::zero()};
        static constexpr auto INPUT_BUFFER_TIME = std::chrono::milliseconds(5000);
        void push()
        {
            if (!m_buffered)
            {
                queuedAt = Clock::now();
                m_buffered = true;
            }
        }

        [[nodiscard]] bool expired() const
        {
            return m_buffered && (Clock::now() - queuedAt) >= INPUT_BUFFER_TIME;
        }

        [[nodiscard]] bool active() const
        {
            return m_buffered && !expired();
        }

        void consume()
        {
            m_buffered = false;
        }

        void clear_if_expired()
        {
            if (expired())
                m_buffered = false;
        }

    };


    struct AttackHandler
    {
        private:
        static void ProcessButton(RE::AttackBlockHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);
        inline static REL::HookVFT _attackBlockHandlerHook{RE::VTABLE_AttackBlockHandler[0], 0x4, ProcessButton};
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
