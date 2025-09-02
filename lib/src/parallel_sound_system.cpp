#include <rst/__service/sound/parallel_sound_system.h>


namespace rst
{
    // TODO: assess code
    ParallelSoundSystem::ParallelSoundSystem( std::unique_ptr<SoundSystem>&& ss )
        : playback_queue_{ {} }
        , impl_ptr_{ std::move( ss ) }
    {
        create_worker_thread( );
    }


    ParallelSoundSystem::~ParallelSoundSystem( )
    {
        running_ = false;

        if ( worker_thread_.joinable( ) )
        {
            loop_cv_.notify_all( );
            worker_thread_.join( );
        }
    }


    auto ParallelSoundSystem::get_service_type( ) -> ServiceType
    {
        return impl_ptr_->get_service_type( );
    }


    auto ParallelSoundSystem::load_sound(
        std::filesystem::path const& path, sound::SoundType const type, Uid const tag_id ) -> std::shared_ptr<Audio>
    {
        return impl_ptr_->load_sound( path, type, tag_id );
    }


    auto ParallelSoundSystem::play( Audio const& audio, float const volume, int const loops ) -> int
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::PLAY,
            .audio = &audio,
            .volume = volume,
            .loops = loops
        } );
        loop_cv_.notify_all( );
        return -1;
    }


    auto ParallelSoundSystem::stop( Audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::STOP,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    auto ParallelSoundSystem::stop_all( ) -> void
    {
        impl_ptr_->stop_all( );
    }


    auto ParallelSoundSystem::pause( Audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::PAUSE,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    auto ParallelSoundSystem::resume( Audio const& audio ) -> bool
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::RESUME,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    auto ParallelSoundSystem::is_playing( Audio const& audio ) const -> bool
    {
        return impl_ptr_->is_playing( audio );
    }


    auto ParallelSoundSystem::is_paused( Audio const& audio ) const -> bool
    {
        return impl_ptr_->is_paused( audio );
    }


    auto ParallelSoundSystem::get_current_track( ) const -> Audio const*
    {
        return impl_ptr_->get_current_track( );
    }


    auto ParallelSoundSystem::set_master_volume( float const volume ) -> void
    {
        return impl_ptr_->set_master_volume( volume );
    }


    auto ParallelSoundSystem::get_master_volume( ) const -> float
    {
        return impl_ptr_->get_master_volume( );
    }


    auto ParallelSoundSystem::set_volume_by_tag( Uid const tag_id, float const volume ) -> void
    {
        return impl_ptr_->set_volume_by_tag( tag_id, volume );
    }


    auto ParallelSoundSystem::get_volume_by_tag( Uid const tag_id ) const -> float
    {
        return impl_ptr_->get_volume_by_tag( tag_id );
    }


    auto ParallelSoundSystem::create_worker_thread( ) -> void
    {
        worker_thread_ = std::thread(
            [this]
                {
                    while ( running_ )
                    {
                        auto [lock, queue] = playback_queue_.get_unique( );
                        if ( sound::SoundPlaybackOptions options; queue.pop( options ) )
                        {
                            switch ( options.mode )
                            {
                                case sound::PlaybackMode::PLAY:
                                    impl_ptr_->play( *options.audio, options.volume, options.loops );
                                    break;

                                case sound::PlaybackMode::STOP:
                                    impl_ptr_->stop( *options.audio );
                                    break;

                                case sound::PlaybackMode::PAUSE:
                                    impl_ptr_->pause( *options.audio );
                                    break;

                                case sound::PlaybackMode::RESUME:
                                    impl_ptr_->resume( *options.audio );
                                    break;
                            }
                        }
                        loop_cv_.wait( lock, [&queue, this] { return not queue.empty( ) || not running_; } );
                    }
                } );
    }


}
