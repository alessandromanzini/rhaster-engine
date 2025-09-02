#ifndef RST_SERVICE_LOCATOR_H
#define RST_SERVICE_LOCATOR_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>
#include <rst/__service/sound/sound_system.h>


namespace rst
{
    class ServiceLocator final : public Singleton<ServiceLocator>
    {
        friend class Singleton;

    public:
        auto register_sound_system( std::unique_ptr<SoundSystem>&& ss ) -> SoundSystem&;
        [[nodiscard]] auto get_sound_system( ) const -> SoundSystem&;
        [[nodiscard]] auto is_sound_system_registered( ) const -> bool;

    private:
        std::unique_ptr<SoundSystem> sound_system_instance_ptr_{};

        ServiceLocator( ) = default;
    };


    extern ServiceLocator& SERVICE_LOCATOR;
}


#endif //!RST_SERVICE_LOCATOR_H
