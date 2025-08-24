// ReSharper disable CppInconsistentNaming
#ifndef UICOMPONENT_H
#define UICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Deletable.h>


namespace engine
{
    class UIController;
    class UIComponent : public Deletable
    {
    public:
        ~UIComponent( ) override = default;

        UIComponent( const UIComponent& )                = delete;
        UIComponent( UIComponent&& ) noexcept            = delete;
        UIComponent& operator=( const UIComponent& )     = delete;
        UIComponent& operator=( UIComponent&& ) noexcept = delete;

        virtual void tick( ) { }
        virtual void render( ) { } // UI needs the render to be non-const

        void mark_for_deletion( ) final;

    protected:
        explicit UIComponent( UIController& owner );

        [[nodiscard]] UIController& get_owner( ) const;

    private:
        UIController& owner_ref_;

    };

}

#endif //!UICOMPONENT_H
