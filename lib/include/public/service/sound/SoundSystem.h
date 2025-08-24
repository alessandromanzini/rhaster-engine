#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <core/UID.h>
#include <framework/resource/sound/Audio.h>
#include <service/service_info.h>

#include <filesystem>


namespace engine
{
    namespace sound
    {
        enum class QueuePolicy : uint8_t
        {
            DISCARD,        // Don't play the new sound if the queue is full
            REPLACE_OLDEST, // Remove the oldest queued sound to play the new one
            REPLACE_NEWEST  // Remove the newest queued sound to play the new one
        };

        enum class ChannelType : uint8_t
        {
            MONO   = 1,
            STEREO = 2
        };

        enum class SampleRate : uint32_t
        {
            HZ_8000   = 8000,
            HZ_11025  = 11025,
            HZ_16000  = 16000,
            HZ_22050  = 22050,
            HZ_32000  = 32000,
            HZ_44100  = 44100,
            HZ_48000  = 48000,
            HZ_96000  = 96000,
            HZ_192000 = 192000
        };
    }

    class SoundSystem
    {
    public:
        SoundSystem( )          = default;
        virtual ~SoundSystem( ) = default;

        SoundSystem( const SoundSystem& )                = delete;
        SoundSystem( SoundSystem&& ) noexcept            = delete;
        SoundSystem& operator=( const SoundSystem& )     = delete;
        SoundSystem& operator=( SoundSystem&& ) noexcept = delete;

        /**
         * Retrieves the underlying service type.
         * @return
         */
        [[nodiscard]] virtual ServiceType get_service_type( ) = 0;

        /**
         * Registers a sound file within the sound system for future use in playback.
         * @param path Storage path to the sound file
         * @param type Type of the sound. SOUND_EFFECT for quick sounds or SOUND_TRACK for longer music tracks.
         * @param tagId
         */
        [[nodiscard]] virtual std::shared_ptr<Audio> load_sound( const std::filesystem::path& path, sound::SoundType type, UID tagId ) = 0;

        /**
         * Requests the sound system to play a sound.
         * @param audio
         * @param volume [0.0-1.0] overrides the default volume
         * @param loops Repeats the sound this amount of times, -1 for infinite loop
         * @return Channel id of the sound being played, -1 if the sound is not playing
         */
        virtual int play( const Audio& audio, float volume, int loops = 0 ) = 0;

        /**
         * Halts the playback of a sound.
         * @param audio
         * @return true if the sound was playing and has been stopped, false if it was not playing
         */
        virtual bool stop( const Audio& audio ) = 0;

        /**
         * Halts the playback of all sounds.
         */
        virtual void stop_all( ) = 0;

        /**
         * Pauses the playback of a sound. Does nothing if the sound is not playing.
         * @param audio
         * @return true if the sound was playing and has been paused, false if it was not playing
         */
        virtual bool pause( const Audio& audio ) = 0;

        /**
         * Resumes the playback of a sound. Plays the sound if it was not playing.
         * @param audio
         * @return true if the sound was paused and has been resumed, false if it was not paused
         */
        virtual bool resume( const Audio& audio ) = 0;

        /**
         * Checks if the sound is currently playing.
         * @param audio
         * @return true if the sound is playing, false otherwise (paused or stopped)
         */
        [[nodiscard]] virtual bool is_playing( const Audio& audio ) const = 0;

        /**
         * Checks if the sound is currently paused.
         * @param audio
         * @return true if the sound is paused, false otherwise (playing or stopped)
         */
        [[nodiscard]] virtual bool is_paused( const Audio& audio ) const = 0;

        /**
         * Fetches the current background music track ID.
         * @return
         */
        [[nodiscard]] virtual const Audio* get_current_track( ) const = 0;

        /**
         * Sets the volume coefficient that applies to all sounds.
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual void set_master_volume( float volume ) = 0;

        /**
         * Gets the current master volume.
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual float get_master_volume( ) const = 0;

        /**
         * Sets the volume coefficient that applies to the tag grouping. It combines with the master volume.
         * @param tagId
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual void set_volume_by_tag( UID tagId, float volume ) = 0;

        /**
         * Gets the current volume for a tag grouping.
         * @param tagId
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual float get_volume_by_tag( UID tagId ) const = 0;

    };


}


#endif //!SOUNDSYSTEM_H
