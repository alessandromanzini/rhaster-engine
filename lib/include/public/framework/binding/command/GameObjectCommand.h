#ifndef GAMEOBJECTCOMMAND_H
#define GAMEOBJECTCOMMAND_H

#include <framework/binding/command/Command.h>


namespace engine
{
    class GameObject;
    class GameObjectCommand : public Command
    {
    public:
        explicit GameObjectCommand( GameObject& object ) : object_ref_{ object } { }

        void execute( ) override { }

        const GameObject& get_object( ) const { return object_ref_; }
        GameObject& get_object( ) { return object_ref_; }

    private:
        GameObject& object_ref_;

    };

}


#endif //!GAMEOBJECTCOMMAND_H
