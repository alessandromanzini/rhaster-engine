#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <vec2.hpp>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <string>

struct SDL_Texture;


namespace engine
{
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Texture2D final
    {
    public:
        explicit Texture2D( SDL_Texture* texture );
        explicit Texture2D( const std::string& fullPath );
        ~Texture2D( ) noexcept;

        Texture2D( const Texture2D& )                = delete;
        Texture2D( Texture2D&& ) noexcept            = delete;
        Texture2D& operator=( const Texture2D& )     = delete;
        Texture2D& operator=( Texture2D&& ) noexcept = delete;

        [[nodiscard]] SDL_Texture* get_SDL_texture( ) const;

        [[nodiscard]] glm::vec2 get_size( ) const;

    private:
        SDL_Texture* texture_ptr_;

    };
}

#endif //!TEXTURE2D_H
