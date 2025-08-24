#ifndef COMPONENT_H
#define COMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Deletable.h>


namespace engine
{
    class GameObject;
    class Component : public Deletable
    {
    public:
        using owner_t = GameObject;

        ~Component( ) override;

        Component( const Component& )                = delete;
        Component( Component&& ) noexcept            = delete;
        Component& operator=( const Component& )     = delete;
        Component& operator=( Component&& ) noexcept = delete;

        virtual void fixed_tick( ) { }
        virtual void tick( ) { }
        virtual void render( ) const { }

        [[nodiscard]] owner_t& get_owner( ) const;

        void mark_for_deletion( ) final;

    protected:
        explicit Component( owner_t& owner );

        virtual void begin_owner_deletion( ) { }

    private:
        owner_t& owner_ref_;

    };

}

#endif //!COMPONENT_H
