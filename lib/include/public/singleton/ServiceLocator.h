#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <singleton/Singleton.h>

#include <framework/controller/PlayerController.h>
#include <service/sound/SoundSystem.h>

#include <memory>


namespace engine
{
    class ServiceLocator final : public Singleton<ServiceLocator>
    {
        friend class Singleton;

    public:
        SoundSystem& register_sound_system( std::unique_ptr<SoundSystem>&& ss );
        [[nodiscard]] SoundSystem& get_sound_system() const;
        [[nodiscard]] bool is_sound_system_registered() const;

    private:
        std::unique_ptr<SoundSystem> sound_system_instance_ptr_{};

        ServiceLocator( )  = default;

    };


    extern ServiceLocator& SERVICE_LOCATOR;

}


#endif //!SERVICELOCATOR_H
