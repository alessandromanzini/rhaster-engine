#include <rst/__type/sound/audio.h>

#include <rst/temp/singleton/resource_manager.h>


namespace rst
{
    auto audio::sound_mark( ) const -> earmark
    {
        return sound_mark_;
    }


    auto audio::tag_mark( ) const -> earmark
    {
        return tag_mark_;
    }


    auto audio::type( ) const -> sound::sound_type
    {
        return type_;
    }


    audio::audio( sound::sound_type const type, earmark const sound_mark, earmark const tag_mark )
        : type_{ type }
        , sound_mark_{ sound_mark }
        , tag_mark_{ tag_mark } { }

}
