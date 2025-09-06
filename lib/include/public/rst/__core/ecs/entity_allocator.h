#ifndef RST_ENTITY_ALLOCATOR_H
#define RST_ENTITY_ALLOCATOR_H

#include <rst/pch.h>

#include <rst/__core/ecs/entity.h>


namespace rst::ecs // todo: add multicast_delegate
{
    class entity_allocator final
    {
    public:
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
        static constexpr entity_type first_entity_{ 1U };
        entity_type next_entity_{ first_entity_ };
    };
}


#endif //!RST_ENTITY_ALLOCATOR_H