// ReSharper disable CppInconsistentNaming
#ifndef UICONTROLLER_H
#define UICONTROLLER_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singleton/Singleton.h>

#include <core/type_utility.hpp>
#include <framework/resource/data/Deleter.h>

#include <core/meta/concepts.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <map>
#include <stdexcept>


union SDL_Event;

namespace engine
{
    class UIComponent;
    class UIController final : public Singleton<UIController>
    {
        friend class Singleton;

    public:
        void init( );
        void destroy( );

        void process_input( const SDL_Event& event );

        void tick( );
        void render( );

        void cleanup( );

        template <typename component_t, typename... args_t>
            requires meta::DerivedUIComponentWithBaseContructor<component_t, args_t...>
        component_t& add_ui_component( args_t&&... args );

        template <typename component_t>
            requires meta::DerivedUIComponent<component_t>
        [[nodiscard]] component_t& get_ui_component( ) const;

        template <typename component_t>
            requires meta::DerivedUIComponent<component_t>
        [[nodiscard]] component_t& get_ui_components( ) const;

        void remove_ui_component( UIComponent& component );

    private:
        std::multimap<uint64_t, std::unique_ptr<UIComponent>> ui_components_{};
        Deleter<UIComponent> deleter_{};

        UIController( ) = default;

        void start_imgui_render( ) const;
        void end_imgui_render( ) const;

    };


    template <typename component_t, typename... args_t>
        requires meta::DerivedUIComponentWithBaseContructor<component_t, args_t...>
    component_t& UIController::add_ui_component( args_t&&... args )
    {
        // Initializing component with GameObjectView and arguments' perfect forwarding.
        // We insert the element in the multimap with its compile-time type hash as the key.
        auto uiComponent = std::make_unique<component_t>( *this, std::forward<args_t>( args )... );
        auto it = ui_components_.insert( std::make_pair( type_utility::type_hash<component_t>( ), std::move( uiComponent ) ) );
        return reinterpret_cast<component_t&>( *( it->second ) );
    }


    template <typename component_t>
        requires meta::DerivedUIComponent<component_t>
    [[nodiscard]] component_t& UIController::get_ui_component( ) const
    {
        // We find the component and reinterpret_cast it to the correct type.
        // This is safe because we know the type is correct.
        auto it = ui_components_.find( type_utility::type_hash<component_t>( ) );
        assert( it != ui_components_.end( ) && "UIComponent not found!" );
        return reinterpret_cast<component_t&>( *it->second.get( ) );
    }


    template <typename component_t>
        requires meta::DerivedUIComponent<component_t>
    [[nodiscard]] component_t& UIController::get_ui_components( ) const
    {
        throw std::runtime_error( "Not implemented!" );
    }


    extern UIController& UI_CONTROLLER;

}

#endif // UICONTROLLER_H
