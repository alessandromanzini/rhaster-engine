#include <rst/temp/singleton/game_time.h>


using namespace std::chrono;

namespace rst
{
    auto game_time::tick( ) -> void
    {
        auto const current_time = high_resolution_clock::now( );

        delta_time_ = duration<float>( current_time - last_time_ ).count( );

        last_time_ = current_time;
        lag_ += delta_time_;

        handle_delegates( timeouts_, true );
        handle_delegates( intervals_, false );
    }


    auto game_time::fixed_tick( ) -> void
    {
        lag_ -= fixed_time_step_;
    }


    auto game_time::reset( ) -> void
    {
        last_time_ = high_resolution_clock::now( );
        lag_       = 0.f;
    }


    auto game_time::set_timing_type( time::timing_type const type ) -> void
    {
        switch ( type )
        {
            case time::timing_type::delta_time:
                current_delta_ptr_ = &delta_time_;
                break;
            case time::timing_type::fixed_delta_time:
                current_delta_ptr_ = &fixed_time_step_;
                break;
        }
    }


    auto game_time::delta_time( ) const -> float
    {
        return *current_delta_ptr_;
    }


    auto game_time::fps( ) const -> float
    {
        auto const fps{ 1.f / delta_time_ };
        return fps;
    }


    auto game_time::is_fixed_tick_required( ) const -> bool
    {
        return lag_ >= fixed_time_step_;
    }


    auto game_time::sleep_time( ) const -> nanoseconds
    {
        return last_time_ + milliseconds( ms_per_frame_ ) - high_resolution_clock::now( );
    }


    auto game_time::set_interval( float const seconds, std::function<bool( )>&& delegate ) -> void
    {
        intervals_.emplace_back( std::make_pair( time::time_span{ seconds, seconds }, std::move( delegate ) ) );
    }


    auto game_time::set_interval( float const seconds, std::function<void( )> const& delegate ) -> void
    {
        set_interval( seconds, std::function{
                          [delegate]( ) -> bool
                          {
                              delegate( );
                              return false;
                          }
                      } );
    }


    auto game_time::set_timeout( float const seconds, std::function<void( )>&& delegate ) -> void
    {
        timeouts_.emplace_back( std::make_pair( time::time_span{ seconds, seconds }, std::move( delegate ) ) );
    }


    auto game_time::clear_timeouts( ) -> void
    {
        // TODO> implement timer specific clear
        timeouts_.clear();
    }


    game_time& GAME_TIME = game_time::instance( );
}
