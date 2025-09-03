#ifndef RST_HARE_H
#define RST_HARE_H

#include <rst/pch.h>


namespace rst
{
    class hare final
    {
    public:
        explicit hare(
            std::string const& window_title, std::filesystem::path const& data_path, glm::vec2 viewport = { 640U, 480U } );
        ~hare( ) noexcept;

        hare( hare const& )                        = delete;
        hare( hare&& ) noexcept                    = delete;
        auto operator=( hare const& ) -> hare&     = delete;
        auto operator=( hare&& ) noexcept -> hare& = delete;

        auto run( ) -> void;

    private:
        glm::vec2 const viewport_;
        bool request_quit_{ false };

        auto run_one_frame( ) -> void;
    };
}


#endif //!RST_HARE_H
