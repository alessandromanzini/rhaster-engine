#include <rst/__input/input_merge.h>


namespace rst::input
{
    auto apply_modifiers_to_value( bool const value, ModifierBitset const& modifiers ) -> InputValueVariant
    {
        return apply_modifiers_to_value( static_cast<float>( value ), modifiers );
    }


    auto apply_modifiers_to_value( float value, ModifierBitset const& modifiers ) -> InputValueVariant
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


    auto merge_value_to_snapshot( InputSnapshot& snapshot, InputValueVariant const value ) -> void
    {
        // TODO: improve this with a priority system.
        std::visit( [&]( auto&& start, auto&& toAdd )
                        {
                            using start_t  = std::decay_t<decltype(start)>;
                            using to_add_t = std::decay_t<decltype(toAdd)>;

                            // we make sure that vec2 remains the relevant type.
                            if constexpr ( std::is_same_v<to_add_t, glm::vec2> )
                            {
                                // please change this it's horrible
                                snapshot.value = convert_input_value<glm::vec2>( start ) + toAdd;
                            }
                            // if dealing with a base boolean, we convert to bool instead
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
