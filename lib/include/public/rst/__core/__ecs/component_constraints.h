#ifndef RST_ECS_COMPONENT_CONSTRAINTS_H
#define RST_ECS_COMPONENT_CONSTRAINTS_H

#include <rst/pch.h>


namespace rst::ecs::detail
{
    template <typename T>
    concept viewable_ecs_component = not std::is_reference_v<T> && not std::is_pointer_v<T> && std::is_trivially_copyable_v<T>;


    template <typename T>
    concept ecs_component = viewable_ecs_component<T> && not std::is_const_v<T>;
}


#endif //!RST_ECS_COMPONENT_CONSTRAINTS_H
