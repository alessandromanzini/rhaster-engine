#include <rst/__core/ecs/entity_allocator.h>


namespace rst::ecs
{
    entity_allocator::entity_allocator( ) { }


    auto entity_allocator::create( ) -> entity_type
    {
        entity_type const entity = token_gen_.generate( );
        on_creation.broadcast( entity );
        return entity;
    }


    // ReSharper disable once CppMemberFunctionMayBeConst
    auto entity_allocator::destroy( entity_type const entity ) -> void
    {
        on_destruction.broadcast( entity );
    }


    auto entity_allocator::clear( ) -> void
    {
        token_gen_.reset( );
        on_clear.broadcast( );
    }
}
