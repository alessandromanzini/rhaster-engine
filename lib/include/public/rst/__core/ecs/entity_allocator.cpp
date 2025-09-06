#include <rst/__core/ecs/entity_allocator.h>


namespace rst::ecs
{
    entity_allocator::entity_allocator( )
    {
    }


    auto entity_allocator::create( ) -> entity_type
    {
        return next_entity_++;
    }


    auto entity_allocator::destroy( entity_type const entity ) -> void
    {
        (void)entity;
        // call destroy listeners
    }


    auto entity_allocator::clear( ) -> void
    {
        next_entity_ = first_entity_;
        // call clear listeners
    }
}
