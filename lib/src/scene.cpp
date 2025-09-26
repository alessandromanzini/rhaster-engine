// #include <rst/__core/scene.h>
//
// #include <rst/__core/gameobject.h>
//
//
// namespace rst
// {
//     scene::scene( std::string name )
//         : name_{ std::move( name ) }
//         , id_( s_id_counter_++ ) { }
//
//
//     auto scene::add( std::unique_ptr<gameobject> object ) -> void
//     {
//         assert( object != nullptr && "Object pointer cannot be null!" );
//         objects_.emplace_back( std::move( object ) );
//     }
//
//
//     auto scene::remove( gameobject& object ) -> void
//     {
//         deleter_.mark_element_for_deletion( &object );
//
//         // remove children as well
//         for ( auto* child : object.children( ) )
//         {
//             remove( *child );
//         }
//     }
//
//
//     auto scene::remove_all( ) -> void
//     {
//         for ( auto& object : objects_ )
//         {
//             deleter_.mark_element_for_deletion( object.get( ) );
//         }
//     }
//
//
//     auto scene::find_object( std::function<bool( gameobject const& )> const& predicate ) const -> OptionalRef<gameobject>
//     {
//         for ( auto const& object : objects_ )
//         {
//             if ( predicate( *object ) )
//             {
//                 return *object;
//             }
//         }
//         return nullptr;
//     }
//
//
//     auto scene::create_object( ) -> gameobject&
//     {
//         return *objects_.emplace_back( std::make_unique<gameobject>( *this ) );
//     }
//
//
//     auto scene::name( ) const -> std::string const&
//     {
//         return name_;
//     }
//
//
//     auto scene::id( ) const -> uint16_t
//     {
//         return id_;
//     }
//
//
//     auto scene::fixed_tick( ) const -> void
//     {
//         for ( auto const& object : objects_ )
//         {
//             object->fixed_tick( );
//         }
//     }
//
//
//     auto scene::tick( ) const -> void
//     {
//         for ( auto const& object : objects_ )
//         {
//             object->tick( );
//         }
//     }
//
//
//     auto scene::render( ) const -> void
//     {
//         for ( auto const& object : objects_ )
//         {
//             object->render( );
//         }
//     }
//
//
//     auto scene::cleanup( ) -> void
//     {
//         // delete marked objects
//         if ( deleter_.is_cleanup_needed( ) )
//         {
//             deleter_.cleanup( objects_ );
//         }
//
//         // cleanup objects
//         for ( auto const& object : objects_ )
//         {
//             object->cleanup( );
//         }
//     }
//
//
//     uint16_t scene::s_id_counter_ = 0;
// }
