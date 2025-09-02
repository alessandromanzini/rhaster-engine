#ifndef RST_SOUND_SYSTEM_H
#define RST_SOUND_SYSTEM_H

#include <rst/pch.h>

#include <rst/__service/service_info.h>
#include <rst/__type/sound_type/audio.h>


namespace rst
{
    namespace sound
    {
        enum class QueuePolicy : uint8_t
        {
            // don't play the new sound if the queue is full
            DISCARD,

            // remove the oldest queued sound to play the new one
            REPLACE_OLDEST,

            // remove the newest queued sound to play the new one
            REPLACE_NEWEST
        };

        enum class ChannelType : uint8_t
        {
            MONO = 1U, STEREO = 2U
        };

        enum class SampleRate : uint32_t
        {
            HZ_8000   = 8000U,
            HZ_11025  = 11025U,
            HZ_16000  = 16000U,
            HZ_22050  = 22050U,
            HZ_32000  = 32000U,
            HZ_44100  = 44100U,
            HZ_48000  = 48000U,
            HZ_96000  = 96000U,
            HZ_192000 = 192000U
        };
    }


    class SoundSystem
    {
    public:
        SoundSystem( )          = default;
        virtual ~SoundSystem( ) = default;

        SoundSystem( SoundSystem const& )                        = delete;
        SoundSystem( SoundSystem&& ) noexcept                    = delete;
        auto operator=( SoundSystem const& ) -> SoundSystem&     = delete;
        auto operator=( SoundSystem&& ) noexcept -> SoundSystem& = delete;

        /**
         * Retrieves the underlying service type.
         * @return
         */
        [[nodiscard]] virtual auto get_service_type( ) -> ServiceType = 0;

        /**
         * Registers a sound file within the sound system for future use in playback.
         * @param path Storage path to the sound file
         * @param type Type of the sound. SOUND_EFFECT for quick sounds or SOUND_TRACK for longer music tracks.
         * @param tag_id
         */
        [[nodiscard]] virtual auto load_sound(
            std::filesystem::path const& path, sound::SoundType type, UID tag_id ) -> std::shared_ptr<Audio> = 0;

        /**
         * Requests the sound system to play a sound.
         * @param audio
         * @param volume [0.0-1.0] overrides the default volume
         * @param loops Repeats the sound this amount of times, -1 for infinite loop
         * @return Channel id of the sound being played, -1 if the sound is not playing
         */
        virtual auto play( Audio const& audio, float volume, int loops = 0 ) -> int = 0;

        /**
         * Halts the playback of a sound.
         * @param audio
         * @return true if the sound was playing and has been stopped, false if it was not playing
         */
        virtual auto stop( Audio const& audio ) -> bool = 0;

        /**
         * Halts the playback of all sounds.
         */
        virtual auto stop_all( ) -> void = 0;

        /**
         * Pauses the playback of a sound. Does nothing if the sound is not playing.
         * @param audio
         * @return true if the sound was playing and has been paused, false if it was not playing
         */
        virtual auto pause( Audio const& audio ) -> bool = 0;

        /**
         * Resumes the playback of a sound. Plays the sound if it was not playing.
         * @param audio
         * @return true if the sound was paused and has been resumed, false if it was not paused
         */
        virtual auto resume( Audio const& audio ) -> bool = 0;

        /**
         * Checks if the sound is currently playing.
         * @param audio
         * @return true if the sound is playing, false otherwise (paused or stopped)
         */
        [[nodiscard]] virtual auto is_playing( Audio const& audio ) const -> bool = 0;

        /**
         * Checks if the sound is currently paused.
         * @param audio
         * @return true if the sound is paused, false otherwise (playing or stopped)
         */
        [[nodiscard]] virtual auto is_paused( Audio const& audio ) const -> bool = 0;

        /**
         * Fetches the current background music track ID.
         * @return
         */
        [[nodiscard]] virtual auto get_current_track( ) const -> Audio const* = 0;

        /**
         * Sets the volume coefficient that applies to all sounds.
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual auto set_master_volume( float volume ) -> void = 0;

        /**
         * Gets the current master volume.
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual auto get_master_volume( ) const -> float = 0;

        /**
         * Sets the volume coefficient that applies to the tag grouping. It combines with the master volume.
         * @param tag_id
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual auto set_volume_by_tag( UID tag_id, float volume ) -> void = 0;

        /**
         * Gets the current volume for a tag grouping.
         * @param tag_id
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual auto get_volume_by_tag( UID tag_id ) const -> float = 0;
    };
}


#endif //!RST_SOUND_SYSTEM_H
