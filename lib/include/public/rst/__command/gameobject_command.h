#ifndef RST_GAMEOBJECT_COMMAND_H
#define RST_GAMEOBJECT_COMMAND_H

#include <rst/__command/command.h>


namespace rst
{
    class gameobject;
}
// todo: remove this
namespace rst
{
    class gameobject_command : public command
    {
    public:
        explicit gameobject_command( gameobject& object ) : object_ref_{ object } { }

        [[nodiscard]] auto object( ) const -> gameobject const& { return object_ref_; }
        [[nodiscard]] auto object( ) -> gameobject& { return object_ref_; }

    private:
        gameobject& object_ref_;
    };
}


#endif //!RST_GAMEOBJECT_COMMAND_H
