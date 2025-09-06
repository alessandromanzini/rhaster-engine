#ifndef RST_SDL_AUDIO_H
#define RST_SDL_AUDIO_H

#include <rst/pch.h>

#include <rst/__resource_type/sound/audio.h>

#include <SDL_mixer.h>


namespace rst
{
    namespace sound
    {
        using resource_type = std::variant<Mix_Chunk*, Mix_Music*>;
    }


    class sdl_audio final : public audio
    {
    public:
        /**
         * Creates an audio resource from a file.
         * @exception runtime_error If the file cannot be loaded
         * @param path Path to the sound file
         * @param type Type of sound (SOUND_EFFECT or SOUND_TRACK)
         * @param sound_mark
         * @param tag_mark
         */
        sdl_audio( std::filesystem::path const& path, sound::sound_type type, earmark sound_mark, earmark tag_mark );
        ~sdl_audio( ) override;

        sdl_audio( sdl_audio const& )                        = delete;
        sdl_audio( sdl_audio&& ) noexcept                    = delete;
        auto operator=( sdl_audio const& ) -> sdl_audio&     = delete;
        auto operator=( sdl_audio&& ) noexcept -> sdl_audio& = delete;

        [[nodiscard]] auto resource( ) const -> sound::resource_type;

    private:
        sound::resource_type resource_;
    };
}


#endif //!RST_SDL_AUDIO_H
