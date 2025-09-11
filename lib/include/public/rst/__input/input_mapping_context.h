#ifndef RST_INPUT_MAPPING_CONTEXT_H
#define RST_INPUT_MAPPING_CONTEXT_H

#include <rst/pch.h>

#include <rst/meta/tuple.h>
#include <rst/meta/type_traits.h>
#include <rst/__input/binding_device.h>
#include <rst/__input/input_type.h>


namespace rst
{
    class command;
    class player_controller;
}

namespace rst
{
    namespace internal
    {
        template <typename TMethod> concept derived_player_controller =
                std::derived_from<typename meta::function_traits<TMethod>::class_t, player_controller>;
    }


    /**
     * This class takes care of abstracting input codes from input actions, allowing the registered devices to receive an input
     * as a named event. Additionally, it applies the requested modifiers on the input automatically before signaling it.
     */
    class input_mapping_context final
    {
        using context_range_type = std::list<input::device_context>;

    public:
        input_mapping_context( )           = default;
        ~input_mapping_context( ) noexcept = default;

        input_mapping_context( input_mapping_context const& )                        = delete;
        input_mapping_context( input_mapping_context&& ) noexcept                    = delete;
        auto operator=( input_mapping_context const& ) -> input_mapping_context&     = delete;
        auto operator=( input_mapping_context&& ) noexcept -> input_mapping_context& = delete;

        /**
         * Registers an input action as a UID bound to the given key code and modifiers.
         * @param action
         * @param code
         */
        auto register_input_action( input::input_action const& action, input::unicode code ) -> void;

        auto register_device( player_controller& controller, input::device_info device_info ) -> void;
        auto unregister_device( player_controller const& controller ) -> void;

        // bind a member function to the input action of the given code. Command will be called once the input action is signaled.
        template <typename TMethod> requires internal::derived_player_controller<TMethod>
        auto bind_to_input_action(
            meta::function_traits<TMethod>::class_t* controller, earmark mark, TMethod command,
            input::trigger trigger = input::trigger::triggered ) -> void;

        // TODO: make this
        auto unbind_from_input_action( player_controller& controller, earmark mark ) -> void;

        // signals the input action bound to the given code and trigger for the correct device.
        auto signal( input::unicode code, input::trigger trigger, input::device_info device_info ) -> void;

        // dispatches the merged signaled events to the corresponding commands.
        auto dispatch( ) -> void;

        [[nodiscard]] auto devices( ) const -> context_range_type const&;

    private:
        std::unordered_map<input::unicode, std::vector<input::input_action>, input::unicode_hasher> action_binds_{};

        context_range_type device_contexts_{};

        auto bind_to_input_action_impl(
            player_controller const& controller, earmark mark, input::input_command_type&& command,
            input::trigger trigger ) -> void;
    };


    template <typename TMethod> requires internal::derived_player_controller<TMethod>
    auto input_mapping_context::bind_to_input_action(
        typename meta::function_traits<TMethod>::class_t* controller, earmark mark, TMethod command,
        input::trigger trigger ) -> void
    {
        assert( controller && "InputMappingContext::bind_to_input_action: controller cannot be nullptr!" );

        using traits_t = meta::function_traits<TMethod>;
        using param_t  = meta::safe_tuple_element_t<0, typename traits_t::params_t, bool>;

        static_assert(
            traits_t::arity <= 1, "InputMappingContext::bind_to_input_action only supports one/zero-parameter functions." );

        std::function<void( param_t )> wrapper = [=]( [[maybe_unused]] auto param )
        {
            if constexpr ( traits_t::arity == 0 )
            {
                std::invoke( command, controller );
            }
            else
            {
                std::invoke( command, controller, param );
            }
        };

        // ReSharper disable once CppDFANullDereference
        this->bind_to_input_action_impl( *controller, mark, wrapper, trigger );
    }
}


#endif //!RST_INPUT_MAPPING_CONTEXT_H
