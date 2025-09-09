#ifndef RST_META_REFERENCE_H
#define RST_META_REFERENCE_H

#include <rst/pch.h>


namespace rst::meta
{
    template <class T>
    concept non_reference = not std::is_reference_v<T>;
}


#endif //!RST_META_REFERENCE_H
