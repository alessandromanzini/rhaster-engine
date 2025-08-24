#ifndef BINDINGDEVICE_H
#define BINDINGDEVICE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <core/UID.h>
#include <framework/binding/binding_types.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <deque>
#include <list>
#include <optional>


namespace engine
{
    class PlayerController;
}


namespace engine::binding
{
    /**
     * This class holds the registered commands for every trigger event and is not bound to another specific object.
     * Binding should be achieved via other control structures.
     */
    class CommandSet final
    {
        using command_list_t = std::list<input_command_variant_t>;

    public:
        void set( CommandInfo&& info );
        void execute( input_value_variant_t value, TriggerEvent trigger ) const;

    private:
        command_list_t triggered_commands_{};
        command_list_t pressed_commands_{};
        command_list_t released_commands_{};

        [[nodiscard]] const command_list_t& select_command_list( TriggerEvent trigger ) const;

    };


    struct DeviceInfo
    {
        DeviceType type{};
        uint8_t id{ std::numeric_limits<decltype( id )>::max( ) };
    };


    /**
     * This class represents the combination of a digital device to a PlayerController. It holds the queue of inputs,
     * merging as they get signaled. It also holds the commands bound for the controller, dispatching them in one
     * iteration from the input's queue.
     */
    class DeviceContext final
    {
    public:
        DeviceContext( PlayerController& controller, DeviceInfo deviceInfo );

        [[nodiscard]] PlayerController& get_controller( );
        [[nodiscard]] const PlayerController& get_controller( ) const;

        [[nodiscard]] bool is_device_suitable( DeviceInfo deviceInfo ) const;

        void bind_command( UID uid, CommandInfo&& commandInfo );
        void unbind_command( UID uid );

        /**
         * Signals the input action linked to the command, if previously bound. It will add a new record to the queue or update
         * the value if it already exists.
         * @param input Snapshot of the input action to signal.
         */
        void signal_input( const InputSnapshot& input );

        /**
         * Dispatches all signaled inputs to the corresponding commands.
         */
        void execute_commands( );

        [[nodiscard]] const DeviceInfo& get_device_info( ) const { return device_info_; }

    private:
        const DeviceInfo device_info_{};

        PlayerController& controller_ref_;

        std::deque<InputSnapshot> signaled_inputs_queue_{};
        std::unordered_map<UID, CommandSet> command_sets_{};

        std::optional<decltype(signaled_inputs_queue_)::iterator> find_queued_input( const InputSnapshot& target );

    };

}


#endif //!BINDINGDEVICE_H
