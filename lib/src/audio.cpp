#include <rst/__type/sound_type/audio.h>

#include <rst/temp/singleton/resource_manager.h>


namespace rst
{
    auto Audio::get_sound_id( ) const -> Uid
    {
        return sound_id_;
    }


    auto Audio::get_tag_id( ) const -> Uid
    {
        return tag_id_;
    }


    auto Audio::get_type( ) const -> sound::SoundType
    {
        return type_;
    }


    Audio::Audio( sound::SoundType const type, Uid const sound_id, Uid const tag_id )
        : type_{ type }
        , sound_id_{ sound_id }
        , tag_id_{ tag_id } { }

}
