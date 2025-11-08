#ifndef RST_SERVICE_SERVICE_INFO_H
#define RST_SERVICE_SERVICE_INFO_H


namespace rst::service
{
    enum class service_type
    {
        null = 0U, ///< default, uninitialized service
        internal,  ///< built-in service
        sdl,       ///< sdl-based service
        custom,    ///< custom service (user-defined)
    };
}


#endif //!RST_SERVICE_SERVICE_INFO_H
