#ifndef RST_EVENT_TOKEN_H
#define RST_EVENT_TOKEN_H

#include <rst/pch.h>


namespace rst::event
{
    using delegate_token_type = uint64_t;
    static constexpr delegate_token_type invalid_token{ 0U };
}


#endif //!RST_EVENT_TOKEN_H