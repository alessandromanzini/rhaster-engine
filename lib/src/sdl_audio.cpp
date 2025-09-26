#include <rst/__internal/resource/sdl_audio.h>

#include <rst/diagnostic.h>


namespace rst
{
    sdl_audio::sdl_audio(
        std::filesystem::path const& path, sound::sound_type const type, earmark const sound_mark, earmark const tag_mark )
        : audio{ type, sound_mark, tag_mark }
    {
        ensure( std::filesystem::exists( path ), "sound file does not exist!" );

        if ( type == sound::sound_type::sound_effect )
        {
            Mix_Chunk* chunk = Mix_LoadWAV( path.string( ).c_str( ) );
            if ( chunk == nullptr )
            {
                startle( "failed to load chunk: {}. error: {}", path.string( ), Mix_GetError( ) );
            }
            resource_ = chunk;
        }
        else if ( type == sound::sound_type::sound_track )
        {
            Mix_Music* music = Mix_LoadMUS( path.string( ).c_str( ) );
            if ( music == nullptr )
            {
                startle( "failed to load music: {}. error: {}", path.string( ), Mix_GetError( ) );
            }
            resource_ = music;
        }
        else
        {
            ensure( false, "invalid sound type!" );
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
