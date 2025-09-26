#ifndef RST_ECS_REGISTRY_POOL_H
#define RST_ECS_REGISTRY_POOL_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/__core/__ecs/entity.h>


namespace rst::ecs::detail
{
    using base_reg_pool_type = base_sparse_set<entity_type>;

    template <typename TComponent>
    using reg_pool_type = sparse_set<TComponent, entity_type>;
}


#endif //!RST_ECS_REGISTRY_POOL_H
