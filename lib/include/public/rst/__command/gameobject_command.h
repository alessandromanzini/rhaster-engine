#ifndef RST_GAMEOBJECT_COMMAND_H
#define RST_GAMEOBJECT_COMMAND_H

#include <rst/__command/command.h>


namespace rst
{
    class GameObject;
}

namespace rst
{
    class GameObjectCommand : public Command
    {
    public:
        explicit GameObjectCommand( GameObject& object ) : object_ref_{ object } { }

        [[nodiscard]] auto object( ) const -> GameObject const& { return object_ref_; }
        [[nodiscard]] auto object( ) -> GameObject& { return object_ref_; }

    private:
        GameObject& object_ref_;
    };
}


#endif //!RST_GAMEOBJECT_COMMAND_H
