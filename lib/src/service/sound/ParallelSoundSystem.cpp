#include <service/sound/ParallelSoundSystem.h>


namespace engine
{
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


    ServiceType ParallelSoundSystem::get_service_type( )
    {
        return impl_ptr_->get_service_type( );
    }


    std::shared_ptr<Audio> ParallelSoundSystem::load_sound( const std::filesystem::path& path,
                                                            const sound::SoundType type,
                                                            const UID tagId )
    {
        return impl_ptr_->load_sound( path, type, tagId );
    }


    int ParallelSoundSystem::play( const Audio& audio, const float volume, const int loops )
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


    bool ParallelSoundSystem::stop( const Audio& audio )
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::STOP,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    void ParallelSoundSystem::stop_all( )
    {
        impl_ptr_->stop_all( );
    }


    bool ParallelSoundSystem::pause( const Audio& audio )
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::PAUSE,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    bool ParallelSoundSystem::resume( const Audio& audio )
    {
        auto [lock, queue] = playback_queue_.get( );
        queue.push( sound::SoundPlaybackOptions{
            .mode = sound::PlaybackMode::RESUME,
            .audio = &audio
        } );
        loop_cv_.notify_all( );
        return false;
    }


    bool ParallelSoundSystem::is_playing( const Audio& audio ) const
    {
        return impl_ptr_->is_playing( audio );
    }


    bool ParallelSoundSystem::is_paused( const Audio& audio ) const
    {
        return impl_ptr_->is_paused( audio );
    }


    const Audio* ParallelSoundSystem::get_current_track( ) const
    {
        return impl_ptr_->get_current_track( );
    }


    void ParallelSoundSystem::set_master_volume( const float volume )
    {
        return impl_ptr_->set_master_volume( volume );
    }


    float ParallelSoundSystem::get_master_volume( ) const
    {
        return impl_ptr_->get_master_volume( );
    }


    void ParallelSoundSystem::set_volume_by_tag( const UID tagId, const float volume )
    {
        return impl_ptr_->set_volume_by_tag( tagId, volume );
    }


    float ParallelSoundSystem::get_volume_by_tag( const UID tagId ) const
    {
        return impl_ptr_->get_volume_by_tag( tagId );
    }


    void ParallelSoundSystem::create_worker_thread( )
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
