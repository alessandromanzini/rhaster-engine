#ifndef RST_SDL_AUDIO_H
#define RST_SDL_AUDIO_H

#include <rst/pch.h>

#include <rst/__type/sound_type/audio.h>

#include <SDL_mixer.h>


namespace rst
{
    namespace sound
    {
        using resource_variant_t = std::variant<Mix_Chunk*, Mix_Music*>;
    }

    class SdlAudio final : public Audio
    {
    public:
        /**
         * Creates an audio resource from a file.
         * @exception runtime_error If the file cannot be loaded
         * @param path Path to the sound file
         * @param type Type of sound (SOUND_EFFECT or SOUND_TRACK)
         * @param sound_id
         * @param tag_id
         */
        SdlAudio( std::filesystem::path const& path, sound::SoundType type, Uid sound_id, Uid tag_id );
        ~SdlAudio( ) override;

        SdlAudio( SdlAudio const& )                        = delete;
        SdlAudio( SdlAudio&& ) noexcept                    = delete;
        auto operator=( SdlAudio const& ) -> SdlAudio&     = delete;
        auto operator=( SdlAudio&& ) noexcept -> SdlAudio& = delete;

        [[nodiscard]] auto get_resource( ) const -> sound::resource_variant_t;

    private:
        sound::resource_variant_t resource_;

    };

}


#endif //!SDLAUDIO_H
