#ifndef RST_INPUT_INPUT_MERGE_H
#define RST_INPUT_INPUT_MERGE_H

#include <rst/pch.h>

#include <rst/meta/function_traits.h>
#include <rst/__input/input_type.h>


// todo: assess names and structure
namespace rst::input
{
    // +---------------------------+
    // | MERGING                   |
    // +---------------------------+
    auto apply_modifiers_to_value( bool value, ModifierBitset const& modifiers ) -> InputValueVariant;
    auto apply_modifiers_to_value( float value, ModifierBitset const& modifiers ) -> InputValueVariant;

    auto merge_value_to_snapshot( InputSnapshot& snapshot, InputValueVariant value ) -> void;


    [[nodiscard]] constexpr auto trigger_to_value( TriggerEvent const trigger ) -> bool
    {
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
            case TriggerEvent::PRESSED: return true;
            case TriggerEvent::RELEASED: return false;
        }
        throw std::invalid_argument( "Invalid trigger event!" );
    }


    template <typename TCastTarget>
    [[nodiscard]] auto convert_input_value( InputValueVariant value ) -> TCastTarget
    {
        return std::visit(
            []<typename T>( T&& arg ) -> TCastTarget
            {
                using ArgType = std::decay_t<T>;

                // default convertible types
                if constexpr ( std::is_convertible_v<ArgType, TCastTarget> )
                {
                    return static_cast<TCastTarget>( arg );
                }

                // from bool/float to vec2
                else if constexpr ( std::is_same_v<TCastTarget, glm::vec2> )
                {
                    return glm::vec2{ static_cast<float>( arg ), 0.f };
                }

                // from vec2 to bool/float
                else if constexpr ( std::is_same_v<ArgType, glm::vec2> )
                {
                    return static_cast<TCastTarget>( static_cast<glm::vec2>( arg ).x );
                }

                else
                {
                    static_assert(
                        meta::BAD_CONVERSION<ArgType, TCastTarget>, "No valid conversion available for arg_t to cast_target_t." );
                }
                return TCastTarget{};
            }, value );
    }
}


#endif //!RST_INPUT_INPUT_MERGE_H
