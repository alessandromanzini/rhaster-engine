#ifndef MINIGIN_H
#define MINIGIN_H

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <filesystem>
#include <functional>


namespace engine
{
    class Minigin final
    {
    public:
        explicit Minigin( const std::filesystem::path& dataPath, uint32_t width = 640, uint32_t height = 480 );
        ~Minigin( ) noexcept;

        Minigin( const Minigin& )                = delete;
        Minigin( Minigin&& ) noexcept            = delete;
        Minigin& operator=( const Minigin& )     = delete;
        Minigin& operator=( Minigin&& ) noexcept = delete;

        void run( const std::function<void( )>& load );
        void run_one_frame( );

    private:
        const uint32_t viewport_width_;
        const uint32_t viewport_height_;

        bool quit_{};

    };

}


#endif //!MINIGIN_H
