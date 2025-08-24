#ifndef AUDIO_H
#define AUDIO_H

#include <core/UID.h>


namespace engine
{
    namespace sound
    {
        enum class SoundType : uint8_t
        {
            SOUND_EFFECT,
            SOUND_TRACK
        };
    }

    class Audio
    {
    public:
        virtual ~Audio( ) = default;

        Audio( const Audio& )                = delete;
        Audio( Audio&& ) noexcept            = delete;
        Audio& operator=( const Audio& )     = delete;
        Audio& operator=( Audio&& ) noexcept = delete;

        [[nodiscard]] UID get_sound_id( ) const;
        [[nodiscard]] UID get_tag_id( ) const;
        [[nodiscard]] sound::SoundType get_type( ) const;

    protected:
        Audio( sound::SoundType type, UID soundId, UID tagId );

    private:
        const sound::SoundType type_{};

        const UID sound_id_{ NULL_UID };
        const UID tag_id_{ NULL_UID };

    };

}


#endif //!AUDIO_H
