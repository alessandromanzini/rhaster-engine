#ifndef RST_PELT_FRAME_H
#define RST_PELT_FRAME_H

#include <rst/pch.h>


namespace rst
{
    class pelt;
}

namespace rst
{
    struct pelt_frame
    {
        pelt const* texture{ nullptr };
        glm::vec4 src_rect{};
        int z_index{ 0U };
    };
}


#endif //!RST_PELT_FRAME_H