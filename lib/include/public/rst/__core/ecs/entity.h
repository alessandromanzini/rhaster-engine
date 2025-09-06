#ifndef RST_ECS_ENTITY_H
#define RST_ECS_ENTITY_H

#include <rst/pch.h>


namespace rst::ecs
{
    using entity_type = uint32_t;
    static constexpr entity_type null_entity{ 0U };
}


#endif //!RST_ECS_ENTITY_H
