#ifndef RST_SERVICE_INFO_H
#define RST_SERVICE_INFO_H


namespace rst
{
    enum class service_type
    {
        // built-in service
        internal = 0U,

        // custom service (user-defined)
        custom = 1U,

        // sdl-based service
        sdl = 2U,
    };
}


#endif //!RST_SERVICE_INFO_H
