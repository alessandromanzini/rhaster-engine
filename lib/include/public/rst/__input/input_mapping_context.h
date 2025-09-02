#ifndef RST_INPUT_MAPPING_CONTEXT_H
#define RST_INPUT_MAPPING_CONTEXT_H

#include <rst/pch.h>

#include <rst/meta/function_traits.h>
#include <rst/__input/binding_device.h>
#include <rst/__input/input_type.h>


namespace rst
{
    class Command;
    class PlayerController;
}

namespace rst
{
    /**
     * This class takes care of abstracting input codes from input actions, allowing the registered devices to receive an input
     * as a named event. Additionally, it applies the requested modifiers on the input automatically before signaling it.
     */
    class InputMappingContext final
    {
        using DeviceContextContainer = std::list<input::DeviceContext>;
        using OptionalDeviceIt       = std::optional<DeviceContextContainer::iterator>;

    public:
        InputMappingContext( )           = default;
        ~InputMappingContext( ) noexcept = default;

        InputMappingContext( InputMappingContext const& )                        = delete;
        InputMappingContext( InputMappingContext&& ) noexcept                    = delete;
        auto operator=( InputMappingContext const& ) -> InputMappingContext&     = delete;
        auto operator=( InputMappingContext&& ) noexcept -> InputMappingContext& = delete;

        /**
         * Registers an input action as a UID bound to the given key code and modifiers.
         * @param action
         * @param code
         */
        auto register_input_action( input::InputAction const& action, input::UniformBindingCode code ) -> void;

        auto register_device( PlayerController& controller, input::DeviceInfo device_info ) -> void;
        auto unregister_device( PlayerController const& controller ) -> void;

        // bind a member function to the input action of the given code. Command will be called once the input action is signaled.
        template <typename TMethod> requires std::derived_from<typename meta::function_traits<TMethod>::class_t, PlayerController>
        auto bind_to_input_action(
            meta::function_traits<TMethod>::class_t* controller, Uid uid, TMethod command,
            input::TriggerEvent trigger = input::TriggerEvent::TRIGGERED ) -> void;

        // TODO: make this
        auto unbind_from_input_action( PlayerController& controller, Uid uid ) -> void;

        // signals the input action bound to the given code and trigger for the correct device.
        auto signal( input::UniformBindingCode code, input::TriggerEvent trigger, input::DeviceInfo device_info ) -> void;

        // dispatches the merged signaled events to the corresponding commands.
        auto dispatch( ) -> void;

        [[nodiscard]] auto get_devices( ) const -> DeviceContextContainer const&;

    private:
        std::unordered_map<input::UniformBindingCode, std::vector<input::InputAction>, input::UniformBindingCodeHasher>
        action_binds_{};

        DeviceContextContainer device_contexts_{};

        auto bind_to_input_action_impl(
            PlayerController const& controller, Uid uid, input::InputCommandVariant&& command,
            input::TriggerEvent trigger ) -> void;
    };


    template <typename TMethod> requires std::derived_from<typename meta::function_traits<TMethod>::class_t, PlayerController>
    auto InputMappingContext::bind_to_input_action(
        typename meta::function_traits<TMethod>::class_t* controller, Uid uid, TMethod command,
        input::TriggerEvent trigger ) -> void
    {
        assert( controller && "InputMappingContext::bind_to_input_action: controller cannot be nullptr!" );

        using traits_t = meta::function_traits<TMethod>;
        using param_t  = meta::safe_tuple_element_t<0, typename traits_t::params_t, bool>;

        static_assert(
            traits_t::ARITY <= 1, "InputMappingContext::bind_to_input_action only supports one/zero-parameter functions." );

        std::function<void( param_t )> wrapper = [=]( [[maybe_unused]] auto param )
        {
            if constexpr ( traits_t::ARITY == 0 )
            {
                std::invoke( command, controller );
            }
            else
            {
                std::invoke( command, controller, param );
            }
        };

        // reSharper disable once CppDFANullDereference
        bind_to_input_action_impl( *controller, uid, wrapper, trigger );
    }
}


#endif //!RST_INPUT_MAPPING_CONTEXT_H
