#ifndef GAMETIME_H
#define GAMETIME_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singleton/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <chrono>
#include <functional>
#include <list>


namespace engine
{
    namespace time
    {
        enum class TimingType
        {
            DELTA_TIME,
            FIXED_DELTA_TIME
        };

        template <typename span_t>
        struct TimeSpan
        {
            const span_t max;
            span_t current;
        };
    }

    class GameTime final : public Singleton<GameTime>
    {
        friend class Singleton;

        template <typename return_t>
        using timed_delegate_t = std::pair<time::TimeSpan<float>, std::function<return_t( )>>;

    public:
        void tick( );
        void fixed_tick( );
        void reset( );

        void set_timing_type( time::TimingType type );

        [[nodiscard]] float get_delta_time( ) const;
        [[nodiscard]] float get_fps( ) const;

        [[nodiscard]] bool is_fixed_tick_required( ) const;
        [[nodiscard]] std::chrono::nanoseconds get_sleep_time( ) const;

        /**
         * Register a delegate to be repeated every amount seconds until the delegate return true.
         * @param seconds Interval in seconds between calls
         * @param delegate
         */
        void set_interval( float seconds, std::function<bool( )>&& delegate );

        /**
         * Register a delegate to be repeated every amount seconds indefinitely.
         * @param seconds Interval in seconds between calls
         * @param delegate
         */
        void set_interval( float seconds, const std::function<void( )>& delegate );

        /**
         * Register a delegate to be called once after the amount of seconds.
         * @param seconds Interval in seconds before the call
         * @param delegate
         */
        void set_timeout( float seconds, std::function<void( )>&& delegate );
        void clear_timeouts( );

    private:
        static constexpr int MS_PER_FRAME_{ 16 };
        static constexpr float FIXED_TIME_STEP_{ 0.005f };

        float delta_time_{ 0.f };
        const float* current_delta_ptr_{ &delta_time_ };

        std::chrono::high_resolution_clock::time_point last_time_{};
        float lag_{ 0.f };

        std::list<timed_delegate_t<bool>> intervals_{};
        std::list<timed_delegate_t<void>> timeouts_{};

        GameTime( ) = default;

        template <typename delegate_return_t>
        void handle_delegates( std::list<timed_delegate_t<delegate_return_t>>& delegates, const bool discardFinished ) const;

    };


    template <typename delegate_return_t>
    void GameTime::handle_delegates( std::list<timed_delegate_t<delegate_return_t>>& delegates,
                                     const bool discardFinished ) const
    {
        for ( auto& [span, delegate] : delegates )
        {
            span.current -= delta_time_;

            if ( span.current <= 0.f )
            {
                bool shouldReset = false;

                if constexpr ( std::is_void_v<delegate_return_t> )
                {
                    delegate( );
                    shouldReset = !discardFinished;
                }
                else
                {
                    shouldReset = !discardFinished || delegate( );
                }

                if ( shouldReset )
                {
                    span.current = span.max;
                }
            }
        }

        std::erase_if( delegates, []( auto& pair )
                           {
                               return pair.first.current <= 0.f;
                           } );
    }


    extern GameTime& GAME_TIME;

}

#endif // GAMETIME_H
