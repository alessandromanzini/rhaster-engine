#ifndef RST_INPUT_BINDING_DEVICE_H
#define RST_INPUT_BINDING_DEVICE_H

#include <rst/pch.h>

#include <rst/__input/input_type.h>


namespace rst
{
    class PlayerController;
}


// todo: assess names and structure
// todo: try to remove CommandSet
namespace rst::input
{
    /**
     * This class holds the registered commands for every trigger event and is not bound to another specific object.
     * Binding should be achieved via other control structures.
     */
    class CommandSet final
    {
        using CommandList = std::list<InputCommandVariant>;

    public:
        auto set( CommandInfo&& info ) -> void;
        auto execute( InputValueVariant value, TriggerEvent trigger ) const -> void;

    private:
        CommandList triggered_commands_{};
        CommandList pressed_commands_{};
        CommandList released_commands_{};

        [[nodiscard]] auto select_command_list( TriggerEvent trigger ) const -> CommandList const&;
    };


    /**
     * This class represents the information about a digital device.
     */
    struct DeviceInfo
    {
        DeviceType type{};
        DeviceId id{ std::numeric_limits<DeviceId>::max( ) };
    };


    /**
     * This class represents the combination of a digital device to a PlayerController. It holds the queue of inputs,
     * merging as they get signaled. It also holds the commands bound for the controller, dispatching them in one
     * iteration from the input's queue.
     */
    class DeviceContext final
    {
    public:
        DeviceContext( PlayerController& controller, DeviceInfo device_info );

        [[nodiscard]] auto get_controller( ) -> PlayerController&;
        [[nodiscard]] auto get_controller( ) const -> PlayerController const&;

        [[nodiscard]] auto is_device_suitable( DeviceInfo device_info ) const -> bool;

        /**
         * Bind a command under a specific UID pool.
         * @param uid
         * @param command_info
         */
        auto bind_command( Uid uid, CommandInfo&& command_info ) -> void;

        /**
         * Signals the input action linked to the command, if previously bound. It will add a new record to the queue or update
         * the value if it already exists.
         * @param input Snapshot of the input action to signal.
         */
        auto signal_input( InputSnapshot const& input ) -> void;

        /**
         * Dispatches all signaled inputs to the corresponding commands.
         */
        auto execute_commands( ) -> void;

        [[nodiscard]] auto get_device_info( ) const -> DeviceInfo const& { return device_info_; }

    private:
        using QueueType = std::deque<InputSnapshot>;

        DeviceInfo const device_info_{};

        PlayerController& controller_ref_;

        QueueType signaled_inputs_queue_;
        std::unordered_map<Uid, CommandSet> command_sets_;
    };
}


#endif //!RST_INPUT_BINDING_DEVICE_H
