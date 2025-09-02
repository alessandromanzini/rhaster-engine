#ifndef RST_AUDIO_COMPONENT_H
#define RST_AUDIO_COMPONENT_H

#include <rst/pch.h>

#include <rst/framework/resource_type.h>
#include <rst/__core/component.h>


namespace rst
{
    // TODO: consider to not use this as a component and remove shared pointers
    class AudioComponent final : public Component
    {
    public:
        explicit AudioComponent(
            owner_t& owner, std::filesystem::path const& path, sound::SoundType type, Uid tag_id = Uid( "default" ) );
        ~AudioComponent( ) noexcept override;

        AudioComponent( AudioComponent const& )                        = delete;
        AudioComponent( AudioComponent&& ) noexcept                    = delete;
        auto operator=( AudioComponent const& ) -> AudioComponent&     = delete;
        auto operator=( AudioComponent&& ) noexcept -> AudioComponent& = delete;

        auto set_volume( float volume ) -> void;
        [[nodiscard]] auto get_volume( ) const -> float;

        auto increase_volume( float delta ) -> void;

        auto play( int loops = 0 ) const -> void;
        auto set_playback_on_deletion( ) -> void;

        auto begin_owner_deletion( ) -> void override;

    private:
        std::shared_ptr<Audio> audio_ptr_{ nullptr };
        float volume_{ 1.f };
        bool playback_on_deletion_{ false };
    };
}


#endif //!RST_AUDIO_COMPONENT_H
