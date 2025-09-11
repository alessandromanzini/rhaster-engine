#ifndef RST_ECS_ERROR_H
#define RST_ECS_ERROR_H


namespace rst::ecs
{
    enum class ecs_error
    {
        invalid_entity_id, entity_not_found, component_not_found,
    };
}


#endif //!RST_ECS_ERROR_H
