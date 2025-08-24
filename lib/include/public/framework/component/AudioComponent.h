#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/resource/sound/Audio.h>

#include <filesystem>


namespace engine
{
    class AudioComponent final : public Component
    {
    public:
        explicit AudioComponent( owner_t& owner, const std::filesystem::path& path, sound::SoundType type, UID tagId = UID( "default" ) );
        ~AudioComponent( ) noexcept override;

        AudioComponent( const AudioComponent& )                = delete;
        AudioComponent( AudioComponent&& ) noexcept            = delete;
        AudioComponent& operator=( const AudioComponent& )     = delete;
        AudioComponent& operator=( AudioComponent&& ) noexcept = delete;

        void set_volume( float volume );
        [[nodiscard]] float get_volume( ) const;

        void increase_volume( float delta );

        void play( int loops = 0 ) const;
        void set_playback_on_deletion();

        void begin_owner_deletion( ) override;

    private:
        std::shared_ptr<Audio> audio_ptr_{ nullptr };

        float volume_{ 1.0f };

        bool playback_on_deletion_{ false };

    };

}


#endif //!AUDIOCOMPONENT_H
