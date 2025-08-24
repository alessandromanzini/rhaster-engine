#ifndef INPUTMAPPINGCONTEXT_H
#define INPUTMAPPINGCONTEXT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <core/UID.h>
#include <core/meta/binding_traits.h>
#include <framework/binding/binding_device.h>
#include <framework/binding/binding_types.h>
#include <framework/resource/data/SafeResource.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <list>
#include <unordered_map>


namespace engine
{
    class Command;
    class PlayerController;

    /**
     * This class takes care of abstracting input codes from input actions, allowing the registered devices to receive an input
     * as a named event. Additionally, it applies the requested modifiers on the input automatically before signaling it.
     */
    class InputMappingContext final
    {
        using device_contexts_container_t = std::list<binding::DeviceContext>;

    public:
        InputMappingContext( )           = default;
        ~InputMappingContext( ) noexcept = default;

        InputMappingContext( const InputMappingContext& )                = delete;
        InputMappingContext( InputMappingContext&& ) noexcept            = delete;
        InputMappingContext& operator=( const InputMappingContext& )     = delete;
        InputMappingContext& operator=( InputMappingContext&& ) noexcept = delete;

        // Registers an input action as a UID bound to the given key code and modifiers.
        void register_input_action( const binding::InputAction& action, binding::UniformBindingCode code );
        void register_input_action( UID uid, binding::UniformBindingCode code );

        void register_device( PlayerController& controller, binding::DeviceInfo deviceInfo );
        void unregister_device( const PlayerController& controller );

        // Bind a member function to the input action of the given code. Command will be called once the input action is signaled.
        template <typename method_signature_t>
            requires std::derived_from<typename meta::function_traits<method_signature_t>::class_t, PlayerController>
        void bind_to_input_action( typename meta::function_traits<method_signature_t>::class_t* controller,
                                   UID uid,
                                   method_signature_t command,
                                   binding::TriggerEvent trigger = binding::TriggerEvent::TRIGGERED );

        void unbind_from_input_action( PlayerController& controller, UID uid );

        // Signals the input action bound to the given code and trigger for the correct device.
        void signal( binding::UniformBindingCode code, binding::TriggerEvent trigger, binding::DeviceInfo deviceInfo );

        // Dispatches the merged signaled events to the corresponding commands.
        void dispatch( );

        [[nodiscard]] const device_contexts_container_t& get_devices( ) const;

    private:
        std::unordered_map<binding::UniformBindingCode, std::vector<binding::InputAction>,
            binding::UniformBindingCodeHasher> action_binds_{};

        device_contexts_container_t device_contexts_{};

        using optional_device_it = std::optional<device_contexts_container_t::iterator>;
        [[nodiscard]] optional_device_it find_device_context( device_contexts_container_t& contexts,
                                                              const PlayerController& controller ) const;

        void bind_to_input_action_impl( const PlayerController& controller, UID uid,
                                        binding::input_command_variant_t&& command,
                                        binding::TriggerEvent trigger );

    };


    template <typename method_signature_t>
        requires std::derived_from<typename meta::function_traits<method_signature_t>::class_t, PlayerController>
    void InputMappingContext::bind_to_input_action(
        typename meta::function_traits<method_signature_t>::class_t* controller,
        UID uid, method_signature_t command,
        binding::TriggerEvent trigger )
    {
        assert( controller && "InputMappingContext::bind_to_input_action: controller cannot be nullptr!" );

        using traits_t = meta::function_traits<method_signature_t>;
        using param_t  = meta::safe_tuple_element_t<0, typename traits_t::params_t, bool>;

        static_assert(traits_t::ARITY <= 1, "InputMappingContext::bind_to_input_action only supports one/zero-parameter functions.");

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

        // ReSharper disable once CppDFANullDereference
        bind_to_input_action_impl( *controller, uid, wrapper, trigger );
    }

}


#endif //!INPUTMAPPINGCONTEXT_H
