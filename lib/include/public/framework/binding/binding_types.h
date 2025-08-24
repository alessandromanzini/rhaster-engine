#ifndef BINDINGTYPES_H
#define BINDINGTYPES_H

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <core/UID.h>

// +--------------------------------+
// | SDL HEADERS	    			|
// +--------------------------------+
#include <SDL_gamecontroller.h>
#include <SDL_keycode.h>

// +--------------------------------+
// | GLM HEADERS	    			|
// +--------------------------------+
#include <vec2.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <bitset>
#include <cassert>
#include <functional>
#include <variant>


namespace engine::binding
{
    // +--------------------------------+
    // | KEY/BTN TYPES					|
    // +--------------------------------+
    using key_t = SDL_KeyCode;
    using btn_t = SDL_GameControllerButton;


    // +---------------------------+
    // | UNIFORM BINDING CODE      |
    // +---------------------------+
    // This struct generalizes the key binding values
    struct UniformBindingCode final
    {
        UniformBindingCode( ) = default;
        constexpr UniformBindingCode( const key_t key ) : code{ static_cast<decltype(code)>( key ) } { }
        constexpr UniformBindingCode( const btn_t btn ) : code{ static_cast<decltype(code)>( btn ) } { }

        explicit operator uint32_t( ) const { return code; }

        [[nodiscard]] bool operator<( const UniformBindingCode& other ) const noexcept { return code < other.code; }
        [[nodiscard]] bool operator==( const UniformBindingCode& other ) const noexcept { return code == other.code; }

        uint32_t code{};

    };

    struct UniformBindingCodeHasher final
    {
        std::size_t operator()( const UniformBindingCode& ubc ) const noexcept
        {
            return std::hash<uint32_t>{}( ubc.code );
        }
    };


    // +--------------------------------+
    // | ENUMERATIONS					|
    // +--------------------------------+
    namespace detail
    {
        using trigger_mask_t  = uint8_t;
        using modifier_mask_t = uint8_t;
    }

    enum class TriggerEvent : detail::trigger_mask_t
    {
        TRIGGERED = 0x0,
        PRESSED   = 0x1,
        RELEASED  = 0x2,
    };

    enum class Modifier : detail::modifier_mask_t
    {
        NEGATE  = 0x0,
        SWIZZLE = 0x1,
    };

    enum class DeviceType : uint8_t
    {
        KEYBOARD,
        GAMEPAD
    };


    // +--------------------------------+
    // | BITSET TYPES					|
    // +--------------------------------+
    using trigger_bitset_t  = std::bitset<sizeof( detail::trigger_mask_t ) * 8>;
    using modifier_bitset_t = std::bitset<sizeof( detail::modifier_mask_t ) * 8>;


    // +--------------------------------+
    // | VARIANTS						|
    // +--------------------------------+
    using input_value_variant_t   = std::variant<bool, float, glm::vec2>;
    using input_command_variant_t = std::variant<std::function<void( bool )>, std::function<void( float )>,
        std::function<void( glm::vec2 )>>;


    // +--------------------------------+
    // | COMPILE-TIME CASTS				|
    // +--------------------------------+
    /**
     * Converts the mask to a sequence number.
     * @param mask Mask to convert
     * @return Sequencial number representing the bit position
     */
    [[nodiscard]] constexpr uint32_t mask_to_seq( uint32_t mask )
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
    [[nodiscard]] constexpr size_t bit_cast( TriggerEvent trigger )
    {
        return static_cast<size_t>( trigger );
    }


    /**
     * Casts the sequence enum value to a single bit representation.
     * @param modifier Modifier enum value
     * @return A bit value that represent a mask at the modifier position
     */
    [[nodiscard]] constexpr size_t bit_cast( Modifier modifier )
    {
        return static_cast<size_t>( modifier );
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam args_t TriggerEvent type
     * @param triggers Variadic pack of triggers
     * @return Bitset combination of the arguments
     */
    template <typename... args_t>
        requires (std::same_as<args_t, TriggerEvent> && ...)
    [[nodiscard]] constexpr trigger_bitset_t bitset_cast( args_t... triggers )
    {
        return trigger_bitset_t{ ( ( 1ull << bit_cast( triggers ) ) | ... ) };
    }


    /**
     * Casts the enum to the bitset representation.
     * @tparam args_t Modifier type
     * @param modifiers Variadic pack of modifiers
     * @return Bitset combination of the arguments
     */
    template <typename... args_t>
        requires (std::same_as<args_t, Modifier> && ...)
    [[nodiscard]] constexpr modifier_bitset_t bitset_cast( args_t... modifiers )
    {
        return modifier_bitset_t{ ( ( 1ull << bit_cast( modifiers ) ) | ... ) };
    }


    // +---------------------------+
    // | STRUCTS                   |
    // +---------------------------+
    /**
     * This struct represents an input action identified by an UID and an optional set of modifiers that alter the
     * registered input values.
     */
    struct InputAction final
    {
        UID uid{ 0 };
        modifier_bitset_t modifiers{};


        template <typename... modifiers_t>
        explicit InputAction( const UID uid, modifiers_t... modifierArgs )
            : uid{ uid }
        {
            if constexpr ( sizeof...( modifiers_t ) > 0 )
            {
                modifiers = bitset_cast<modifiers_t...>( modifierArgs... );
            }
        }


        explicit InputAction( const UID uid, const modifier_bitset_t& modifiers )
            : uid{ uid }
            , modifiers{ modifiers } { }

    };


    /**
     * This struct holds the accumulated value for the input action to its corresponding uid and trigger.
     */
    struct InputSnapshot final
    {
        UID uid{ 0 };
        input_value_variant_t value{};
        TriggerEvent trigger{};

        bool operator==( const InputSnapshot& other ) const { return uid == other.uid && trigger == other.trigger; }

    };


    struct CommandInfo final
    {
        input_command_variant_t command;
        TriggerEvent trigger;
    };


}

#endif //!BINDINGTYPES_H
