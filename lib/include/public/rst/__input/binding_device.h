#ifndef RST_INPUT_BINDING_DEVICE_H
#define RST_INPUT_BINDING_DEVICE_H

#include <rst/pch.h>

#include <rst/__input/input_type.h>


namespace rst
{
    class player_controller;
}


// todo: assess names and structure
// todo: try to remove CommandSet
namespace rst::input
{
    /**
     * This class holds the registered commands for every trigger event and is not bound to another specific object.
     * Binding should be achieved via other control structures.
     */
    class command_set final
    {
        using list_type = std::list<input_command_type>;

    public:
        auto set( command_info&& info ) -> void;
        auto execute( input_value_type value, trigger trigger ) const -> void;

    private:
        list_type triggered_commands_{};
        list_type pressed_commands_{};
        list_type released_commands_{};

        [[nodiscard]] auto select_command_list( trigger trigger ) const -> list_type const&;
    };


    /**
     * This class represents the information about a digital device.
     */
    struct device_info
    {
        device_type type{};
        device_id_type id{ std::numeric_limits<device_id_type>::max( ) };
    };


    /**
     * This class represents the combination of a digital device to a PlayerController. It holds the queue of inputs,
     * merging as they get signaled. It also holds the commands bound for the controller, dispatching them in one
     * iteration from the input's queue.
     */
    class device_context final
    {
    public:
        device_context( player_controller& controller, device_info device_info );

        [[nodiscard]] auto controller( ) -> player_controller&;
        [[nodiscard]] auto controller( ) const -> player_controller const&;

        [[nodiscard]] auto is_device_suitable( device_info device_info ) const -> bool;

        /**
         * Bind a command under a specific UID pool.
         * @param mark
         * @param command_info
         */
        auto bind_command( earmark mark, command_info&& command_info ) -> void;

        /**
         * Signals the input action linked to the command, if previously bound. It will add a new record to the queue or update
         * the value if it already exists.
         * @param input Snapshot of the input action to signal.
         */
        auto signal_input( input_reduction const& input ) -> void;

        /**
         * Dispatches all signaled inputs to the corresponding commands.
         */
        auto execute_commands( ) -> void;

        [[nodiscard]] auto device_info( ) const -> device_info const& { return device_info_; }

    private:
        using queue_type = std::deque<input_reduction>;

        input::device_info const device_info_{};

        player_controller& controller_ref_;

        queue_type signaled_inputs_queue_;
        std::unordered_map<earmark, command_set> command_sets_;
    };
}


#endif //!RST_INPUT_BINDING_DEVICE_H
