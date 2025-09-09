#ifndef RST_ECS_COMPONENT_POOL_H
#define RST_ECS_COMPONENT_POOL_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/__core/ecs/entity.h>


namespace rst::ecs::internal
{
    using base_component_pool_type = base_sparse_set<entity_type>;

    template <typename TComponent>
    using component_pool_type = sparse_set<TComponent, entity_type>;
}


#endif //!RST_ECS_COMPONENT_POOL_H
