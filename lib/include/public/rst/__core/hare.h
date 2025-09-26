#ifndef RST_HARE_H
#define RST_HARE_H

#include <rst/pch.h>

#include <rst/__core/__ecs/registry.h>
#include <rst/__core/__service/service_locator.h>
#include <rst/__core/__system/system_scheduler.h>
#include <rst/__core/__system/system_timing.h>


namespace rst
{
    namespace detail
    {
        enum class hop_result : uint8_t
        {
            success = 0U, ///< Successful exit
            flood,        ///< Exhausted resources
            pitfall,      ///< Caught error
            unknown       ///< Unknown error
        };
    }


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

        [[nodiscard]] auto registry( ) noexcept -> ecs::registry&;
        [[nodiscard]] auto service_locator( ) noexcept -> service_locator&;
        [[nodiscard]] auto scheduler( ) noexcept -> system_scheduler<system_timing>&;

        auto run( ) noexcept -> detail::hop_result;

    private:
        glm::vec2 const viewport_;
        bool request_quit_{ false };

        ecs::registry registry_{};
        rst::service_locator service_locator_{};
        system_scheduler<system_timing> scheduler_{ registry_, service_locator_ };

        auto run_one_frame( ) -> void;
    };
}


#endif //!RST_HARE_H
