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
    using KeyCode = SDL_KeyCode;

    /**
     * Gamepad input code type
     */
    using BtnCode = SDL_GameControllerButton;

    /**
     * Fundamental type for device identification
     */
    using DeviceId = uint8_t;

    namespace internal
    {
        using UnderlyingTriggerMask  = uint8_t;
        using UnderlyingModifierMask = uint8_t;
    }
    using TriggerBitset  = std::bitset<sizeof( internal::UnderlyingTriggerMask ) * 8U>;
    using ModifierBitset = std::bitset<sizeof( internal::UnderlyingModifierMask ) * 8U>;

    // TODO: enclose variants in structure
    using InputValueVariant   = std::variant<bool, float, glm::vec2>;
    using InputCommandVariant = std::variant<std::function<void( bool )>, std::function<void( float )>, std::function<void(
        glm::vec2 )>>;


    // +--------------------------------+
    // | ENUMERATIONS                   |
    // +--------------------------------+
    /**
     * Type of input device
     */
    enum class DeviceType : uint8_t
    {
        KEYBOARD, GAMEPAD
    };

    /**
     * Trigger event hooks for input actions
     */
    enum class TriggerEvent : internal::UnderlyingTriggerMask
    {
        TRIGGERED = 0U, PRESSED = 1U << 0U, RELEASED = 1U << 1U,
    };

    /**
     * Modifiers that alter the input action behavior
     */
    enum class Modifier : internal::UnderlyingModifierMask
    {
        NEGATE = 0U, SWIZZLE = 1U << 0U,
    };


    // +---------------------------+
    // | STRUCTS                   |
    // +---------------------------+
    /**
     * This struct generalizes the key binding values
     */
    struct UniformBindingCode final
    {
        UniformBindingCode( ) = default;

        // ReSharper disable CppNonExplicitConvertingConstructor
        constexpr UniformBindingCode( KeyCode const key ) : code{ static_cast<decltype(code)>( key ) } { }
        constexpr UniformBindingCode( BtnCode const btn ) : code{ static_cast<decltype(code)>( btn ) } { }
        // ReSharper restore CppNonExplicitConvertingConstructor

        constexpr explicit operator uint32_t( ) const { return code; }

        constexpr auto operator<( UniformBindingCode const& other ) const noexcept -> bool { return code < other.code; }
        constexpr auto operator==( UniformBindingCode const& other ) const noexcept -> bool { return code == other.code; }

        uint32_t code{};
    };

    struct UniformBindingCodeHasher final
    {
        constexpr auto operator()( UniformBindingCode const& ubc ) const noexcept -> std::size_t
        {
            return std::hash<uint32_t>{}( ubc.code );
        }
    };


    /**
     * This struct represents an input action identified by an UID and an optional set of modifiers that alter the
     * registered input values.
     */
    struct InputAction final
    {
        UID uid{ 0 };
        ModifierBitset modifiers{};


        template <typename... modifiers_t>
        explicit InputAction( UID const uid, modifiers_t... modifier_args )
            : uid{ uid }
        {
            if constexpr ( sizeof...( modifiers_t ) > 0 )
            {
                modifiers = bitset_cast<modifiers_t...>( modifier_args... );
            }
        }


        explicit InputAction( UID const uid, ModifierBitset const& modifiers )
            : uid{ uid }
            , modifiers{ modifiers } { }
    };


    /**
     * This struct holds the accumulated value for the input action to its corresponding uid and trigger.
     */
    struct InputSnapshot final
    {
        UID uid{ 0 };
        InputValueVariant value{};
        TriggerEvent trigger{};

        auto operator==( InputSnapshot const& other ) const -> bool { return uid == other.uid && trigger == other.trigger; }
    };


    // TODO: command info should not be needed
    struct CommandInfo final
    {
        InputCommandVariant command;
        TriggerEvent trigger;
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
    [[nodiscard]] constexpr auto bit_cast( TriggerEvent trigger ) -> size_t
    {
        return static_cast<size_t>( trigger );
    }


    /**
     * Casts the sequence enum value to a single bit representation.
     * @param modifier Modifier enum value
     * @return A bit value that represent a mask at the modifier position
     */
    [[nodiscard]] constexpr auto bit_cast( Modifier modifier ) -> size_t
    {
        return static_cast<size_t>( modifier );
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam args_t TriggerEvent type
     * @param triggers Variadic pack of triggers
     * @return Bitset combination of the arguments
     */
    template <typename... args_t> requires (std::same_as<args_t, TriggerEvent> && ...)
    [[nodiscard]] constexpr auto bitset_cast( args_t... triggers ) -> TriggerBitset
    {
        return TriggerBitset{ ( ( 1ull << bit_cast( triggers ) ) | ... ) };
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam args_t Modifier type
     * @param modifiers Variadic pack of modifiers
     * @return Bitset combination of the arguments
     */
    template <typename... args_t> requires (std::same_as<args_t, Modifier> && ...)
    [[nodiscard]] constexpr auto bitset_cast( args_t... modifiers ) -> ModifierBitset
    {
        return ModifierBitset{ ( ( 1ull << bit_cast( modifiers ) ) | ... ) };
    }
}

#endif //!RST_BINDING_TYPE_H
