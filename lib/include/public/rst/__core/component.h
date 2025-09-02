#ifndef RST_COMPONENT_H
#define RST_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/deletable.h>


namespace rst
{
    class GameObject;
}

// TODO: add documentation
namespace rst
{
    // ReSharper disable once CppClassCanBeFinal
    class Component : public Deletable
    {
    public:
        using owner_t = GameObject;

        ~Component( ) override;

        Component( Component const& )                        = delete;
        Component( Component&& ) noexcept                    = delete;
        auto operator=( Component const& ) -> Component&     = delete;
        auto operator=( Component&& ) noexcept -> Component& = delete;

        virtual auto fixed_tick( ) -> void { }
        virtual auto tick( ) -> void { }
        virtual auto render( ) const -> void { }

        [[nodiscard]] auto get_owner( ) const -> owner_t&;

        auto mark_for_deletion( ) -> void final;

    protected:
        explicit Component( owner_t& owner );

        virtual auto begin_owner_deletion( ) -> void { }

    private:
        owner_t& owner_ref_;
    };
}


#endif //!RST_COMPONENT_H
