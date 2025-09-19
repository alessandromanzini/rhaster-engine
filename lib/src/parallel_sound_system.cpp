#include <rst/__core/__service/sound_service.h>
#include <rst/__core/__service/sound/parallel_sound_service.h>


namespace rst::service
{
    // TODO: assess code
    parallel_sound_service::parallel_sound_service( std::unique_ptr<sound_service>&& ss )
        : playback_queue_{ {} }
        , impl_ptr_{ std::move( ss ) }
    {
        create_worker_thread( );
    }


    parallel_sound_service::~parallel_sound_service( )
    {
        running_ = false;

        if ( worker_thread_.joinable( ) )
        {
            loop_cv_.notify_all( );
            worker_thread_.join( );
        }
    }


    auto parallel_sound_service::service_type( ) -> service::service_type
    {
        return impl_ptr_->service_type( );
    }


    auto parallel_sound_service::load_sound(
        std::filesystem::path const& path, sound::sound_type const type, earmark const tag_id ) -> std::shared_ptr<audio>
    {
        return impl_ptr_->load_sound( path, type, tag_id );
    }


    auto parallel_sound_service::play( audio const& audio, float const volume, int const loops ) -> int
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push(
            sound::sound_playback_options{
                .mode = sound::playback_mode::play,
                .audio = &audio,
                .volume = volume,
                .loops = loops
            } );
        loop_cv_.notify_all( );
        return -1;
    }


    auto parallel_sound_service::stop( audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push(
            sound::sound_playback_options{
                .mode = sound::playback_mode::stop,
                .audio = &audio
            } );
        loop_cv_.notify_all( );
        return false;
    }


    auto parallel_sound_service::stop_all( ) -> void
    {
        impl_ptr_->stop_all( );
    }


    auto parallel_sound_service::pause( audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push(
            sound::sound_playback_options{
                .mode = sound::playback_mode::pause,
                .audio = &audio
            } );
        loop_cv_.notify_all( );
        return false;
    }


    auto parallel_sound_service::resume( audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push(
            sound::sound_playback_options{
                .mode = sound::playback_mode::resume,
                .audio = &audio
            } );
        loop_cv_.notify_all( );
        return false;
    }


    auto parallel_sound_service::is_playing( audio const& audio ) const -> bool
    {
        return impl_ptr_->is_playing( audio );
    }


    auto parallel_sound_service::is_paused( audio const& audio ) const -> bool
    {
        return impl_ptr_->is_paused( audio );
    }


    auto parallel_sound_service::current_track( ) const -> audio const*
    {
        return impl_ptr_->current_track( );
    }


    auto parallel_sound_service::set_master_volume( float const volume ) -> void
    {
        return impl_ptr_->set_master_volume( volume );
    }


    auto parallel_sound_service::master_volume( ) const -> float
    {
        return impl_ptr_->master_volume( );
    }


    auto parallel_sound_service::set_volume_by_tag( earmark const tag_id, float const volume ) -> void
    {
        return impl_ptr_->set_volume_by_tag( tag_id, volume );
    }


    auto parallel_sound_service::volume_by_tag( earmark const tag_id ) const -> float
    {
        return impl_ptr_->volume_by_tag( tag_id );
    }


    auto parallel_sound_service::create_worker_thread( ) -> void
    {
        worker_thread_ = std::thread(
            [this]
            {
                while ( running_ )
                {
                    auto [lock, queue] = playback_queue_.unique( );
                    if ( sound::sound_playback_options options; queue.pop( options ) )
                    {
                        switch ( options.mode )
                        {
                            case sound::playback_mode::play: impl_ptr_->play( *options.audio, options.volume, options.loops );
                                break;

                            case sound::playback_mode::stop: impl_ptr_->stop( *options.audio );
                                break;

                            case sound::playback_mode::pause: impl_ptr_->pause( *options.audio );
                                break;

                            case sound::playback_mode::resume: impl_ptr_->resume( *options.audio );
                                break;
                        }
                    }
                    loop_cv_.wait( lock, [&queue, this] { return not queue.empty( ) || not running_; } );
                }
            } );
    }
}
