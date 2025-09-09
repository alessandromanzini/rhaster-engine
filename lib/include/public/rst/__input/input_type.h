#ifndef RST_BINDING_TYPE_H
#define RST_BINDING_TYPE_H

#include <rst/pch.h>

#include <SDL_gamecontroller.h>
#include <SDL_keycode.h>


// TODO: pimpl sdl
namespace rst::input
{
    // +--------------------------------+
    // | TYPEDEFS                       |
    // +--------------------------------+
    /**
     * Keyboard input code type
     */
    using key_type = SDL_KeyCode;

    /**
     * Gamepad input code type
     */
    using btn_type = SDL_GameControllerButton;

    /**
     * Fundamental type for device identification
     */
    using device_id_type = uint8_t;

    namespace internal
    {
        using underlying_trigger_mask_type  = uint8_t;
        using underlying_modifier_mask_type = uint8_t;
    }
    using trigger_bitset_type  = std::bitset<sizeof( internal::underlying_trigger_mask_type ) * 8U>;
    using modifier_bitset_type = std::bitset<sizeof( internal::underlying_modifier_mask_type ) * 8U>;

    // TODO: enclose variants in structure
    using input_value_type   = std::variant<bool, float, glm::vec2>;
    using input_command_type = std::variant<std::function<void( bool )>, std::function<void( float )>, std::function<void(
        glm::vec2 )>>;


    // +--------------------------------+
    // | ENUMERATIONS                   |
    // +--------------------------------+
    /**
     * Type of input device
     */
    enum class device_type : uint8_t
    {
        keyboard, gamepad
    };

    /**
     * Trigger event hooks for input actions
     */
    enum class trigger : internal::underlying_trigger_mask_type
    {
        triggered = 0U, pressed = 1U << 0U, released = 1U << 1U,
    };

    /**
     * Modifiers that alter the input action behavior
     */
    enum class modifier : internal::underlying_modifier_mask_type
    {
        negate = 0U, swizzle = 1U << 0U,
    };


    // +---------------------------+
    // | STRUCTS                   |
    // +---------------------------+
    /**
     * This struct generalizes the key binding values
     */
    struct unicode final
    {
        unicode( ) = default;

        // ReSharper disable CppNonExplicitConvertingConstructor
        constexpr unicode( key_type const key ) : code{ static_cast<decltype(code)>( key ) } { }
        constexpr unicode( btn_type const btn ) : code{ static_cast<decltype(code)>( btn ) } { }
        // ReSharper restore CppNonExplicitConvertingConstructor

        constexpr explicit operator uint32_t( ) const { return code; }

        constexpr auto operator<( unicode const& other ) const noexcept -> bool { return code < other.code; }
        constexpr auto operator==( unicode const& other ) const noexcept -> bool { return code == other.code; }

        uint32_t code{};
    };

    struct unicode_hasher final
    {
        constexpr auto operator()( unicode const& ubc ) const noexcept -> std::size_t
        {
            return std::hash<uint32_t>{}( ubc.code );
        }
    };


    /**
     * This struct represents an input action identified by an UID and an optional set of modifiers that alter the
     * registered input values.
     */
    struct input_action final
    {
        earmark mark;
        modifier_bitset_type modifiers;


        template <typename... TModifiers>
        explicit input_action( earmark const identifier, TModifiers... modifier_args )
            : mark{ identifier }
        {
            if constexpr ( sizeof...( TModifiers ) > 0 )
            {
                modifiers = bitset_cast<TModifiers...>( modifier_args... );
            }
        }


        explicit input_action( earmark const identifier, modifier_bitset_type const& modifiers )
            : mark{ identifier }
            , modifiers{ modifiers } { }
    };


    /**
     * This struct holds the accumulated value for the input action to its corresponding mark and trigger.
     */
    struct input_reduction final
    {
        earmark mark;
        input_value_type value{};
        trigger hook{};

        auto operator==( input_reduction const& other ) const -> bool { return mark == other.mark && hook == other.hook; }
    };


    // TODO: command info should not be needed
    struct command_info final
    {
        input_command_type command;
        trigger trigger;
    };


    // +--------------------------------+
    // | COMPILE-TIME CASTS             |
    // +--------------------------------+
    /**
     * Converts the mask to a sequence number.
     * @param mask Mask to convert
     * @return Sequencial number representing the bit position
     */
    [[nodiscard]] constexpr auto mask_to_seq( uint32_t mask ) -> uint32_t
    {
        assert( mask != 0 && "Mask cannot be 0!" );

        uint32_t pos = 0;
        while ( ( mask & 1 ) == 0 )
        {
            mask >>= 1;
            pos++;
        }
        return pos;
    }


    /**
     * Casts the sequence enum value to a single bit representation.
     * @param trigger TriggerEvent enum value
     * @return A bit value that represent a mask at the trigger position
     */
    [[nodiscard]] constexpr auto bit_cast( trigger trigger ) -> size_t
    {
        return static_cast<size_t>( trigger );
    }


    /**
     * Casts the sequence enum value to a single bit representation.
     * @param modifier Modifier enum value
     * @return A bit value that represent a mask at the modifier position
     */
    [[nodiscard]] constexpr auto bit_cast( modifier modifier ) -> size_t
    {
        return static_cast<size_t>( modifier );
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam TArgs TriggerEvent type
     * @param triggers Variadic pack of triggers
     * @return Bitset combination of the arguments
     */
    template <typename... TArgs> requires (std::same_as<TArgs, trigger> && ...)
    [[nodiscard]] constexpr auto bitset_cast( TArgs... triggers ) -> trigger_bitset_type
    {
        return trigger_bitset_type{ ( ( 1ULL << bit_cast( triggers ) ) | ... ) };
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam TArgs Modifier type
     * @param modifiers Variadic pack of modifiers
     * @return Bitset combination of the arguments
     */
    template <typename... TArgs> requires (std::same_as<TArgs, modifier> && ...)
    [[nodiscard]] constexpr auto bitset_cast( TArgs... modifiers ) -> modifier_bitset_type
    {
        return modifier_bitset_type{ ( ( 1ULL << bit_cast( modifiers ) ) | ... ) };
    }
}

#endif //!RST_BINDING_TYPE_H
