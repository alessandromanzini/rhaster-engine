// #include <rst/temp/framework/component/audio_component.h>
//
// #include <rst/temp/singleton/resource_manager.h>
// #include <rst/__core/__service/service_locator.h>
//
//
// namespace rst
// {
//     audio_component::audio_component( owner_type& owner, std::filesystem::path const& path, sound::sound_type const type, earmark const tag_id )
//         : component{ owner }
//         , audio_ptr_{ SERVICE_LOCATOR.sound_system( ).load_sound( path, type, tag_id ) } { }
//
//
//     audio_component::~audio_component( ) noexcept
//     {
//         RESOURCE_MANAGER.signal_lifetime_event( event::lifetime_event::unload_audio, audio_ptr_->sound_mark(  ) );
//     }
//
//
//     auto audio_component::set_volume( float const volume ) -> void
//     {
//         volume_ = std::clamp( volume, 0.f, 1.f );
//     }
//
//
//     auto audio_component::volume( ) const -> float
//     {
//         return volume_;
//     }
//
//
//     auto audio_component::increase_volume( float const delta ) -> void
//     {
//         set_volume( volume(  ) + delta );
//     }
//
//
//     auto audio_component::play( int const loops ) const -> void
//     {
//         SERVICE_LOCATOR.sound_system( ).play( *audio_ptr_, volume_, loops );
//     }
//
//
//     auto audio_component::set_playback_on_deletion( ) -> void
//     {
//         playback_on_deletion_ = true;
//     }
//
//
//     auto audio_component::begin_owner_deletion( ) -> void
//     {
//         if ( playback_on_deletion_)
//         {
//             play( );
//         }
//     }
//
// }
