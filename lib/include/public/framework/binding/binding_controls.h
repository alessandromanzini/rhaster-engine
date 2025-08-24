#ifndef BINDINGCONTROLS_H
#define BINDINGCONTROLS_H

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <core/meta/binding_traits.h>
#include <framework/binding/binding_types.h>

// +---------------------------+
// | STANDARD HEADERS          |
// +---------------------------+
#include <stdexcept>


namespace engine::binding
{
    // +---------------------------+
    // | MERGING                   |
    // +---------------------------+
    input_value_variant_t apply_modifiers_to_value( bool value, const modifier_bitset_t& modifiers );
    input_value_variant_t apply_modifiers_to_value( float value, const modifier_bitset_t& modifiers );

    void merge_value_to_snapshot( InputSnapshot& snapshot, input_value_variant_t value );


    [[nodiscard]] constexpr bool trigger_to_value( const TriggerEvent trigger )
    {
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
            case TriggerEvent::PRESSED:
                return true;
            case TriggerEvent::RELEASED:
                return false;
        }
		throw std::invalid_argument( "Invalid trigger event!" );
    }


    template <typename cast_target_t>
    [[nodiscard]] cast_target_t convert_input_value( input_value_variant_t value )
    {
        return std::visit(
            []( auto&& arg ) -> cast_target_t
                {
                    using arg_t = std::decay_t<decltype(arg)>;

                    // Default convertible types
                    if constexpr ( std::is_convertible_v<arg_t, cast_target_t> )
                    {
                        return static_cast<cast_target_t>( arg );
                    }

                    // from bool/float to vec2
                    else if constexpr ( std::is_same_v<cast_target_t, glm::vec2> )
                    {
                        return glm::vec2{ static_cast<float>( arg ), 0.f };
                    }

                    // from vec2 to bool/float
                    else if constexpr ( std::is_same_v<arg_t, glm::vec2> )
                    {
                        return static_cast<cast_target_t>( static_cast<glm::vec2>( arg ).x );
                    }

                    else
                    {
                        static_assert( meta::BAD_CONVERSION<arg_t, cast_target_t>, "No valid conversion available for arg_t to cast_target_t." );
                    }
                    // ReSharper disable once CppNotAllPathsReturnValue
                },
            value
        );
    }

}


#endif //!BINDINGCONTROLS_H
