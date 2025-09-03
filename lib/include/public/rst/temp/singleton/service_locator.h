#ifndef RST_SERVICE_LOCATOR_H
#define RST_SERVICE_LOCATOR_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>
#include <rst/__service/sound/sound_system.h>


// todo: remove singleton
namespace rst
{
    class service_locator final : public singleton<service_locator>
    {
        friend class singleton;

    public:
        auto register_sound_system( std::unique_ptr<sound_system>&& ss ) -> sound_system&;

        [[nodiscard]] auto sound_system( ) const -> sound_system&;
        [[nodiscard]] auto is_sound_system_registered( ) const -> bool;

    private:
        std::unique_ptr<rst::sound_system> sound_system_instance_ptr_{};

        service_locator( ) = default;
    };


    extern service_locator& SERVICE_LOCATOR;
}


#endif //!RST_SERVICE_LOCATOR_H
