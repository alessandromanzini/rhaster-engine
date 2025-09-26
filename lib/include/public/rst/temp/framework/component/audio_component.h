// #ifndef RST_AUDIO_COMPONENT_H
// #define RST_AUDIO_COMPONENT_H
//
// #include <rst/pch.h>
//
// #include <rst/__core/component/component.h>
// #include <rst/__core/resource/audio.h>
//
//
// namespace rst
// {
//     // TODO: consider to not use this as a component and remove shared pointers
//     class audio_component final : public component
//     {
//     public:
//         explicit audio_component(
//             owner_type& owner, std::filesystem::path const& path, sound::sound_type type, earmark tag_mark = earmark( "default" ) );
//         ~audio_component( ) noexcept override;
//
//         audio_component( audio_component const& )                        = delete;
//         audio_component( audio_component&& ) noexcept                    = delete;
//         auto operator=( audio_component const& ) -> audio_component&     = delete;
//         auto operator=( audio_component&& ) noexcept -> audio_component& = delete;
//
//         auto set_volume( float volume ) -> void;
//         [[nodiscard]] auto volume( ) const -> float;
//
//         auto increase_volume( float delta ) -> void;
//
//         auto play( int loops = 0 ) const -> void;
//         auto set_playback_on_deletion( ) -> void;
//
//         auto begin_owner_deletion( ) -> void override;
//
//     private:
//         std::shared_ptr<audio> audio_ptr_{ nullptr };
//         float volume_{ 1.f };
//         bool playback_on_deletion_{ false };
//     };
// }
//
//
// #endif //!RST_AUDIO_COMPONENT_H
