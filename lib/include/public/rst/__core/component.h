#ifndef RST_COMPONENT_H
#define RST_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/deletable.h>


namespace rst
{
    class gameobject;
}

// TODO: add documentation
namespace rst
{
    // ReSharper disable once CppClassCanBeFinal
    class component : public deletable
    {
    public:
        using owner_type = gameobject;

        ~component( ) override;

        component( component const& )                        = delete;
        component( component&& ) noexcept                    = delete;
        auto operator=( component const& ) -> component&     = delete;
        auto operator=( component&& ) noexcept -> component& = delete;

        virtual auto fixed_tick( ) -> void { }
        virtual auto tick( ) -> void { }
        virtual auto render( ) const -> void { }

        [[nodiscard]] auto owner( ) const -> owner_type&;

        auto mark_for_deletion( ) -> void final;

    protected:
        explicit component( owner_type& owner );

        virtual auto begin_owner_deletion( ) -> void { }

    private:
        owner_type& owner_ref_;
    };
}


#endif //!RST_COMPONENT_H
