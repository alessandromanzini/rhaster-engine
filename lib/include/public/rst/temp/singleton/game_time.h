#ifndef RST_GAME_TIME_H
#define RST_GAME_TIME_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>


namespace rst
{
    namespace time
    {
        enum class timing_type
        {
            variable, fixed
        };


        template <typename TSpan>
        struct time_span
        {
            TSpan const max;
            TSpan current;
        };
    }

    class game_time final : public singleton<game_time>
    {
        friend class singleton;

        template <typename TReturn>
        using timed_delegate_type = std::pair<time::time_span<float>, std::function<TReturn( )>>;

    public:
        auto tick( ) -> void;
        auto fixed_tick( ) -> void;
        auto reset( ) -> void;

        auto set_timing_type( time::timing_type type ) -> void;

        [[nodiscard]] auto delta_time( ) const -> float;
        [[nodiscard]] auto fps( ) const -> float;

        [[nodiscard]] auto is_fixed_tick_required( ) const -> bool;
        [[nodiscard]] auto sleep_time( ) const -> std::chrono::nanoseconds;

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
        static constexpr int ms_per_frame_{ 16 };
        static constexpr float fixed_time_step_{ 0.005f };

        float delta_time_{ 0.f };
        float const* current_delta_ptr_{ &delta_time_ };

        std::chrono::high_resolution_clock::time_point last_time_{};
        float lag_{ 0.f };

        // change to array
        std::list<timed_delegate_type<bool>> intervals_{};
        std::list<timed_delegate_type<void>> timeouts_{};

        game_time( ) = default;

        template <typename TReturn>
        auto handle_delegates( std::list<timed_delegate_type<TReturn>>& delegates, bool discard_finished ) const -> void;
    };


    template <typename TReturn>
    auto game_time::handle_delegates(
        std::list<timed_delegate_type<TReturn>>& delegates, bool const discard_finished ) const -> void
    {
        for ( auto& [span, delegate] : delegates )
        {
            span.current -= delta_time_;

            if ( span.current <= 0.f )
            {
                bool should_reset = false;

                if constexpr ( std::same_as<TReturn, void> )
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

        std::erase_if( delegates, []( auto& pair ) { return pair.first.current <= 0.f; } );
    }


    extern game_time& GAME_TIME;
}


#endif //!RST_GAME_TIME_H
