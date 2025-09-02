#include <rst/__type/sound_type/sdl_audio.h>


namespace rst
{
    SdlAudio::SdlAudio( std::filesystem::path const& path, sound::SoundType const type, Uid const sound_id, Uid const tag_id )
        : Audio( type, sound_id, tag_id )
    {
        assert( std::filesystem::exists( path ) && "Sound file does not exist!" );

        if ( get_type(  ) == sound::SoundType::SOUND_EFFECT )
        {
            Mix_Chunk* chunk = Mix_LoadWAV( path.string( ).c_str( ) );
            if ( chunk == nullptr )
            {
                std::stringstream ss{};
                ss << "Failed to load chunk: " << path.string( ) << "\n" << "Error: " << Mix_GetError( );
                throw std::runtime_error( ss.str( ).c_str( ) );
            }
            resource_ = chunk;
        }
        else if ( get_type(  ) == sound::SoundType::SOUND_TRACK )
        {
            Mix_Music* music = Mix_LoadMUS( path.string( ).c_str( ) );
            if ( music == nullptr )
            {
                std::stringstream ss{};
                ss << "Failed to load music: " << path.string( ) << "\n" << "Error: " << Mix_GetError( );
                throw std::runtime_error( ss.str( ).c_str( ) );
            }
            resource_ = music;
        }
        else
        {
            assert( false && "Invalid sound type!" );
        }
    }


    SdlAudio::~SdlAudio( )
    {
        switch ( get_type(  ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                Mix_FreeChunk( std::get<Mix_Chunk*>( resource_ ) );
            break;
            case sound::SoundType::SOUND_TRACK:
                Mix_FreeMusic( std::get<Mix_Music*>( resource_ ) );
            break;
        }
    }


    auto SdlAudio::get_resource( ) const -> sound::resource_variant_t
    {
        return resource_;
    }

}
