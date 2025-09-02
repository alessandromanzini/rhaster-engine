#ifndef RST_SERVICE_INFO_H
#define RST_SERVICE_INFO_H


namespace rst
{
    enum class ServiceType
    {
        // built-in service
        INTERNAL = 0U,

        // custom service (user-defined)
        CUSTOM = 1U,

        // sdl-based service
        SDL = 2U,
    };
}


#endif //!RST_SERVICE_INFO_H
