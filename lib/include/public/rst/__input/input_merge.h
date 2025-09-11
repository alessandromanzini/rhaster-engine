#ifndef RST_INPUT_INPUT_MERGE_H
#define RST_INPUT_INPUT_MERGE_H

#include <rst/pch.h>

#include <rst/meta/type_traits.h>
#include <rst/__input/input_type.h>


// todo: assess names and structure
namespace rst::input
{
    // +---------------------------+
    // | MERGING                   |
    // +---------------------------+
    auto apply_modifiers_to_value( bool value, modifier_bitset_type const& modifiers ) -> input_value_type;
    auto apply_modifiers_to_value( float value, modifier_bitset_type const& modifiers ) -> input_value_type;

    auto merge_value_to_snapshot( input_reduction& snapshot, input_value_type value ) -> void;


    [[nodiscard]] constexpr auto trigger_to_value( trigger const trigger ) -> bool
    {
        switch ( trigger )
        {
            case trigger::triggered:
            case trigger::pressed: return true;
            case trigger::released: return false;
        }
        throw std::invalid_argument( "Invalid trigger event!" );
    }


    template <typename TCastTarget>
    [[nodiscard]] auto convert_input_value( input_value_type value ) -> TCastTarget
    {
        return std::visit(
            []<typename T>( T&& arg ) -> TCastTarget
            {
                using arg_type = std::decay_t<T>;

                // default convertible types
                if constexpr ( std::convertible_to<arg_type, TCastTarget> )
                {
                    return static_cast<TCastTarget>( arg );
                }

                // from bool/float to vec2
                else if constexpr ( std::same_as<TCastTarget, glm::vec2> )
                {
                    return glm::vec2{ static_cast<float>( arg ), 0.f };
                }

                // from vec2 to bool/float
                else if constexpr ( std::same_as<arg_type, glm::vec2> )
                {
                    return static_cast<TCastTarget>( static_cast<glm::vec2>( arg ).x );
                }

                else
                {
                    static_assert(
                        meta::bad_conversion<arg_type, TCastTarget>, "No valid conversion available for arg_t to cast_target_t." );
                }
                return TCastTarget{};
            }, value );
    }
}


#endif //!RST_INPUT_INPUT_MERGE_H
