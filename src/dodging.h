#pragma once

namespace Dodge
{
    struct DoublePress
    {
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        static constexpr auto threshold = std::chrono::milliseconds(300);

        TimePoint last_press{}; // zero = never pressed

        bool on_key_pressed()
        {
            const auto now = Clock::now();

            const bool is_double = last_press != TimePoint{} && (now - last_press) <= threshold;
            last_press = now;
            return is_double;
        }
    };

    struct DodgeInputBuffer
    {
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        static constexpr auto INPUT_BUFFER_TIME = std::chrono::milliseconds(250); // actual window needs testing
        TimePoint queuedAt{Clock::duration::zero()};
        bool queued = false;
        bool checkedOnce = false;

        void push()
        {
            queuedAt = Clock::now();
            queued = true;
        }

        [[nodiscard]] bool expired() const
        {
            if (!queued)
                return true;

            return (Clock::now() - queuedAt) > INPUT_BUFFER_TIME;
        }

        bool consume()
        {
            if (!queued || expired())
            {
                queued = false;
                return false;
            }
            queued = false;
            return true;
        }

        void clear()
        {
            queued = false;
        }
    };

    void OnInput();          // Called by the 3 input methods
    void Update(RE::Actor* a_actor);           // Called in the player update loop
    bool CanDodge(RE::Actor* a_actor);         // main decider if dodge is allowed
    void DoDodge(RE::Actor* a_actor);          // do dodge
}
