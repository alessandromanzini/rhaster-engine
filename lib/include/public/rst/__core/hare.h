#ifndef RST_HARE_H
#define RST_HARE_H

#include <rst/pch.h>


namespace rst
{
    class Hare final
    {
    public:
        explicit Hare(
            std::string const& window_title, std::filesystem::path const& data_path, glm::vec2 viewport = { 640U, 480U } );
        ~Hare( ) noexcept;

        Hare( Hare const& )                        = delete;
        Hare( Hare&& ) noexcept                    = delete;
        auto operator=( Hare const& ) -> Hare&     = delete;
        auto operator=( Hare&& ) noexcept -> Hare& = delete;

        auto run( ) -> void;

    private:
        glm::vec2 const viewport_;
        bool request_quit_{ false };

        auto run_one_frame( ) -> void;
    };
}


#endif //!RST_HARE_H
