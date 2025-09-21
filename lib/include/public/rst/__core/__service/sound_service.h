#ifndef RST_SOUND_SYSTEM_SERVICE_H
#define RST_SOUND_SYSTEM_SERVICE_H

#include <rst/pch.h>

#include <rst/__core/resource/audio.h>
#include <rst/__core/__service/service_info.h>


namespace rst
{
    namespace sound
    {
        enum class queue_policy : uint8_t
        {
            // don't play the new sound if the queue is full
            discard,

            // remove the oldest queued sound to play the new one
            replace_oldest,

            // remove the newest queued sound to play the new one
            replace_newest
        };

        enum class channel_type : uint8_t
        {
            mono = 1U, stereo = 2U
        };

        enum class sample_rate : uint32_t
        {
            hz_8000   = 8000U, hz_11025  = 11025U, hz_16000 = 16000U, hz_22050 = 22050U,
            hz_32000  = 32000U, hz_44100 = 44100U, hz_48000 = 48000U, hz_96000 = 96000U,
            hz_192000 = 192000U
        };
    }


    class sound_service
    {
    public:
        sound_service( )          = default;
        virtual ~sound_service( ) = default;

        sound_service( sound_service const& )                        = delete;
        sound_service( sound_service&& ) noexcept                    = delete;
        auto operator=( sound_service const& ) -> sound_service&     = delete;
        auto operator=( sound_service&& ) noexcept -> sound_service& = delete;

        /**
         * Retrieves the underlying service type.
         * @return
         */
        [[nodiscard]] virtual auto service_type( ) -> service::service_type = 0;

        /**
         * Registers a sound file within the sound system for future use in playback.
         * @param path Storage path to the sound file
         * @param type Type of the sound. SOUND_EFFECT for quick sounds or SOUND_TRACK for longer music tracks
         * @param tag_mark
         */
        [[nodiscard]] virtual auto load_sound(
            std::filesystem::path const& path, sound::sound_type type, earmark tag_mark ) -> std::shared_ptr<audio> = 0;

        /**
         * Requests the sound system to play a sound.
         * @param audio
         * @param volume [0.0-1.0] overrides the default volume
         * @param loops Repeats the sound this amount of times, -1 for infinite loop
         * @return Channel id of the sound being played, -1 if the sound is not playing
         */
        virtual auto play( audio const& audio, float volume, int loops = 0 ) -> int = 0;

        /**
         * Halts the playback of a sound.
         * @param audio
         * @return true if the sound was playing and has been stopped, false if it was not playing
         */
        virtual auto stop( audio const& audio ) -> bool = 0;

        /**
         * Halts the playback of all sounds.
         */
        virtual auto stop_all( ) -> void = 0;

        /**
         * Pauses the playback of a sound. Does nothing if the sound is not playing.
         * @param audio
         * @return true if the sound was playing and has been paused, false if it was not playing
         */
        virtual auto pause( audio const& audio ) -> bool = 0;

        /**
         * Resumes the playback of a sound. Plays the sound if it was not playing.
         * @param audio
         * @return true if the sound was paused and has been resumed, false if it was not paused
         */
        virtual auto resume( audio const& audio ) -> bool = 0;

        /**
         * Checks if the sound is currently playing.
         * @param audio
         * @return true if the sound is playing, false otherwise (paused or stopped)
         */
        [[nodiscard]] virtual auto is_playing( audio const& audio ) const -> bool = 0;

        /**
         * Checks if the sound is currently paused.
         * @param audio
         * @return true if the sound is paused, false otherwise (playing or stopped)
         */
        [[nodiscard]] virtual auto is_paused( audio const& audio ) const -> bool = 0;

        /**
         * Fetches the current background music track ID.
         * @return
         */
        [[nodiscard]] virtual auto current_track( ) const -> audio const* = 0;

        /**
         * Sets the volume coefficient that applies to all sounds.
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual auto set_master_volume( float volume ) -> void = 0;

        /**
         * Gets the current master volume.
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual auto master_volume( ) const -> float = 0;

        /**
         * Sets the volume coefficient that applies to the tag grouping. It combines with the master volume.
         * @param tag_mark
         * @param volume [0.0-1.0] as volume modifier
         */
        virtual auto set_volume_by_tag( earmark tag_mark, float volume ) -> void = 0;

        /**
         * Gets the current volume for a tag grouping.
         * @param tag_mark
         * @return [0.0-1.0] as volume modifier
         */
        [[nodiscard]] virtual auto volume_by_tag( earmark tag_mark ) const -> float = 0;
    };
}


#endif //!RST_SOUND_SYSTEM_SERVICE_H
