#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include <framework/resource/sound/Audio.h>

#include <SDL_mixer.h>

#include <filesystem>
#include <variant>


namespace engine
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
         * @param soundId
         * @param tagId
         */
        SdlAudio( const std::filesystem::path& path, sound::SoundType type, UID soundId, UID tagId );
        ~SdlAudio( ) override;

        SdlAudio( const SdlAudio& )                = delete;
        SdlAudio( SdlAudio&& ) noexcept            = delete;
        SdlAudio& operator=( const SdlAudio& )     = delete;
        SdlAudio& operator=( SdlAudio&& ) noexcept = delete;

        [[nodiscard]] sound::resource_variant_t get_resource() const;

    private:
        sound::resource_variant_t resource_;

    };

}


#endif //!SDLAUDIO_H
