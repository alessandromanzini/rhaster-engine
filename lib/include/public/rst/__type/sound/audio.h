#ifndef RST_AUDIO_H
#define RST_AUDIO_H

#include <rst/pch.h>


namespace rst
{
    namespace sound
    {
        enum class sound_type : uint8_t
        {
            sound_effect, sound_track
        };
    }

    class audio
    {
    public:
        virtual ~audio( ) = default;

        audio( audio const& )                        = delete;
        audio( audio&& ) noexcept                    = delete;
        auto operator=( audio const& ) -> audio&     = delete;
        auto operator=( audio&& ) noexcept -> audio& = delete;

        [[nodiscard]] auto sound_mark( ) const -> earmark;
        [[nodiscard]] auto tag_mark( ) const -> earmark;
        [[nodiscard]] auto type( ) const -> sound::sound_type;

    protected:
        audio( sound::sound_type type, earmark sound_mark, earmark tag_mark );

    private:
        sound::sound_type const type_{};

        earmark const sound_mark_{};
        earmark const tag_mark_{};
    };
}


#endif //!RST_AUDIO_H
