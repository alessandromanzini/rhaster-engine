#ifndef RST_AUDIO_H
#define RST_AUDIO_H

#include <rst/pch.h>


namespace rst
{
    namespace sound
    {
        enum class SoundType : uint8_t
        {
            SOUND_EFFECT, SOUND_TRACK
        };
    }

    class Audio
    {
    public:
        virtual ~Audio( ) = default;

        Audio( Audio const& )                        = delete;
        Audio( Audio&& ) noexcept                    = delete;
        auto operator=( Audio const& ) -> Audio&     = delete;
        auto operator=( Audio&& ) noexcept -> Audio& = delete;

        [[nodiscard]] auto get_sound_id( ) const -> Uid;
        [[nodiscard]] auto get_tag_id( ) const -> Uid;
        [[nodiscard]] auto get_type( ) const -> sound::SoundType;

    protected:
        Audio( sound::SoundType type, Uid sound_id, Uid tag_id );

    private:
        sound::SoundType const type_{};

        Uid const sound_id_{};
        Uid const tag_id_{};
    };
}


#endif //!RST_AUDIO_H
