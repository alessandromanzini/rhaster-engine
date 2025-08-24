#include <framework/binding/binding_controls.h>


namespace engine::binding
{
    input_value_variant_t apply_modifiers_to_value( const bool value, const modifier_bitset_t& modifiers )
    {
        return apply_modifiers_to_value( static_cast<float>( value ), modifiers );
    }


    input_value_variant_t apply_modifiers_to_value( float value, const modifier_bitset_t& modifiers )
    {
        // TODO: optimize the modifier application
        if ( modifiers.test( bit_cast( Modifier::NEGATE ) ) )
        {
            value = -1 * value;
        }

        if ( modifiers.test( bit_cast( Modifier::SWIZZLE ) ) )
        {
            return glm::vec2{ 0.F, value };
        }
        return value;
    }


    void merge_value_to_snapshot( InputSnapshot& snapshot, const input_value_variant_t value )
    {
        // TODO: improve this with a priority system.
        std::visit( [&]( auto&& start, auto&& toAdd )
                        {
                            using start_t  = std::decay_t<decltype(start)>;
                            using to_add_t = std::decay_t<decltype(toAdd)>;

                            // We make sure that vec2 remains the relevant type.
                            if constexpr ( std::is_same_v<to_add_t, glm::vec2> )
                            {
                                // Please change this it's horrible
                                snapshot.value = convert_input_value<glm::vec2>( start ) + toAdd;
                            }
                            // If dealing with a base boolean, we convert to bool instead
                            else if constexpr ( std::is_same_v<start_t, bool> )
                            {
                                snapshot.value = static_cast<float>( start ) + convert_input_value<float>( toAdd );
                            }
                            else
                            {
                                start += convert_input_value<start_t>( toAdd );
                            }
                        }, snapshot.value, value );
    }

}
