#include <rst/temp/singleton/game_time.h>


using namespace std::chrono;

namespace rst
{
    auto GameTime::tick( ) -> void
    {
        auto const current_time = high_resolution_clock::now( );

        delta_time_ = duration<float>( current_time - last_time_ ).count( );

        last_time_ = current_time;
        lag_ += delta_time_;

        handle_delegates( timeouts_, true );
        handle_delegates( intervals_, false );
    }


    auto GameTime::fixed_tick( ) -> void
    {
        lag_ -= FIXED_TIME_STEP_;
    }


    auto GameTime::reset( ) -> void
    {
        last_time_ = high_resolution_clock::now( );
        lag_       = 0.f;
    }


    auto GameTime::set_timing_type( time::TimingType const type ) -> void
    {
        switch ( type )
        {
            case time::TimingType::DELTA_TIME:
                current_delta_ptr_ = &delta_time_;
                break;
            case time::TimingType::FIXED_DELTA_TIME:
                current_delta_ptr_ = &FIXED_TIME_STEP_;
                break;
        }
    }


    auto GameTime::get_delta_time( ) const -> float
    {
        return *current_delta_ptr_;
    }


    auto GameTime::get_fps( ) const -> float
    {
        auto const fps{ 1.f / delta_time_ };
        return fps;
    }


    auto GameTime::is_fixed_tick_required( ) const -> bool
    {
        return lag_ >= FIXED_TIME_STEP_;
    }


    auto GameTime::get_sleep_time( ) const -> nanoseconds
    {
        return last_time_ + milliseconds( MS_PER_FRAME_ ) - high_resolution_clock::now( );
    }


    auto GameTime::set_interval( float const seconds, std::function<bool( )>&& delegate ) -> void
    {
        intervals_.emplace_back( std::make_pair( time::TimeSpan{ seconds, seconds }, std::move( delegate ) ) );
    }


    auto GameTime::set_interval( float const seconds, std::function<void( )> const& delegate ) -> void
    {
        set_interval( seconds, std::function{
                          [delegate]( ) -> bool
                          {
                              delegate( );
                              return false;
                          }
                      } );
    }


    auto GameTime::set_timeout( float const seconds, std::function<void( )>&& delegate ) -> void
    {
        timeouts_.emplace_back( std::make_pair( time::TimeSpan{ seconds, seconds }, std::move( delegate ) ) );
    }


    auto GameTime::clear_timeouts( ) -> void
    {
        // TODO> implement timer specific clear
        timeouts_.clear();
    }


    GameTime& GAME_TIME = GameTime::get_instance( );
}
