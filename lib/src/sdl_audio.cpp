#include <rst/__resource_type/sound/sdl_audio.h>


namespace rst
{
    sdl_audio::sdl_audio(
        std::filesystem::path const& path, sound::sound_type const type, earmark const sound_mark, earmark const tag_mark )
        : audio{ type, sound_mark, tag_mark }
    {
        assert( std::filesystem::exists( path ) && "Sound file does not exist!" );

        if ( type == sound::sound_type::sound_effect )
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
        else if ( type == sound::sound_type::sound_track )
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


    sdl_audio::~sdl_audio( )
    {
        switch ( type( ) )
        {
            case sound::sound_type::sound_effect: Mix_FreeChunk( std::get<Mix_Chunk*>( resource_ ) );
                break;
            case sound::sound_type::sound_track: Mix_FreeMusic( std::get<Mix_Music*>( resource_ ) );
                break;
        }
    }


    auto sdl_audio::resource( ) const -> sound::resource_type
    {
        return resource_;
    }
}
