// ReSharper disable CppInconsistentNaming
#ifndef META_H
#define META_H

#include <framework/component/Component.h>
#include <framework/ui_component/UIComponent.h>

#include <concepts>


namespace engine::meta
{
    // +--------------------------------+
    // | CONCEPTS						|
    // +--------------------------------+
    template <typename derived_t, typename... args_t>
    concept DerivedComponentWithBaseContructor =
            std::derived_from<derived_t, Component> and
            std::constructible_from<derived_t, Component::owner_t&, args_t...>;

    template <typename derived_t>
    concept DerivedComponent = std::derived_from<derived_t, Component>;

    template <typename derived_t, typename... args_t>
    concept DerivedUIComponentWithBaseContructor =
            std::derived_from<derived_t, UIComponent> and
            std::constructible_from<derived_t, UIController&, args_t...>;

    template <typename derived_t>
    concept DerivedUIComponent =
            std::derived_from<derived_t, UIComponent>;

}


#endif //!META_H
