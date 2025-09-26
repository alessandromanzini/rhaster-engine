#ifndef RST_ECS_ENTITY_ALLOCATOR_H
#define RST_ECS_ENTITY_ALLOCATOR_H

#include <rst/pch.h>

#include <rst/data_type/token_generator.h>
#include <rst/data_type/event/multicast_delegate.h>
#include <rst/__core/__ecs/entity.h>


namespace rst::ecs
{
    class entity_allocator final
    {
    public:
        multicast_delegate<entity_type> on_creation{};
        multicast_delegate<entity_type> on_destruction{};
        multicast_delegate<> on_clear{};

        entity_allocator( );
        ~entity_allocator( ) noexcept = default;

        entity_allocator( entity_allocator const& )                        = delete;
        entity_allocator( entity_allocator&& ) noexcept                    = delete;
        auto operator=( entity_allocator const& ) -> entity_allocator&     = delete;
        auto operator=( entity_allocator&& ) noexcept -> entity_allocator& = delete;

        auto create( ) -> entity_type;
        auto destroy( entity_type entity ) -> void;
        auto clear( ) -> void;

    private:
        token_generator<entity_type, null_entity> token_gen_{};
    };
}


#endif //!RST_ECS_ENTITY_ALLOCATOR_H
