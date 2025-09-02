#ifndef RST_GAME_TIME_H
#define RST_GAME_TIME_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>


namespace rst
{
    namespace time
    {
        enum class TimingType
        {
            DELTA_TIME, FIXED_DELTA_TIME
        };

        template <typename TSpan>
        struct TimeSpan
        {
            TSpan const max;
            TSpan current;
        };
    }

    class GameTime final : public Singleton<GameTime>
    {
        friend class Singleton;

        template <typename TReturn>
        using timed_delegate_t = std::pair<time::TimeSpan<float>, std::function<TReturn( )>>;

    public:
        auto tick( ) -> void;
        auto fixed_tick( ) -> void;
        auto reset( ) -> void;

        auto set_timing_type( time::TimingType type ) -> void;

        [[nodiscard]] auto get_delta_time( ) const -> float;
        [[nodiscard]] auto get_fps( ) const -> float;

        [[nodiscard]] auto is_fixed_tick_required( ) const -> bool;
        [[nodiscard]] auto get_sleep_time( ) const -> std::chrono::nanoseconds;

        /**
         * Register a delegate to be repeated every amount seconds until the delegate return true.
         * @param seconds Interval in seconds between calls
         * @param delegate
         */
        auto set_interval( float seconds, std::function<bool( )>&& delegate ) -> void;

        /**
         * Register a delegate to be repeated every amount seconds indefinitely.
         * @param seconds Interval in seconds between calls
         * @param delegate
         */
        auto set_interval( float seconds, std::function<void( )> const& delegate ) -> void;

        /**
         * Register a delegate to be called once after the amount of seconds.
         * @param seconds Interval in seconds before the call
         * @param delegate
         */
        auto set_timeout( float seconds, std::function<void( )>&& delegate ) -> void;
        auto clear_timeouts( ) -> void;

    private:
        static constexpr int MS_PER_FRAME_{ 16 };
        static constexpr float FIXED_TIME_STEP_{ 0.005f };

        float delta_time_{ 0.f };
        float const* current_delta_ptr_{ &delta_time_ };

        std::chrono::high_resolution_clock::time_point last_time_{};
        float lag_{ 0.f };

        std::list<timed_delegate_t<bool>> intervals_{};
        std::list<timed_delegate_t<void>> timeouts_{};

        GameTime( ) = default;

        template <typename TReturn>
        auto handle_delegates( std::list<timed_delegate_t<TReturn>>& delegates, bool discard_finished ) const -> void;
    };


    template <typename TReturn>
    auto GameTime::handle_delegates( std::list<timed_delegate_t<TReturn>>& delegates, bool const discard_finished ) const -> void
    {
        for ( auto& [span, delegate] : delegates )
        {
            span.current -= delta_time_;

            if ( span.current <= 0.f )
            {
                bool should_reset = false;

                if constexpr ( std::is_void_v<TReturn> )
                {
                    delegate( );
                    should_reset = !discard_finished;
                }
                else
                {
                    should_reset = !discard_finished || delegate( );
                }

                if ( should_reset )
                {
                    span.current = span.max;
                }
            }
        }

        std::erase_if(
            delegates, []( auto& pair )
            {
                return pair.first.current <= 0.f;
            } );
    }


    extern GameTime& GAME_TIME;
}


#endif //!RST_GAME_TIME_H
