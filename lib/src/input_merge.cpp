#include <rst/__input/input_merge.h>


namespace rst::input
{
    auto apply_modifiers_to_value( bool const value, modifier_bitset_type const& modifiers ) -> input_value_type
    {
        return apply_modifiers_to_value( static_cast<float>( value ), modifiers );
    }


    auto apply_modifiers_to_value( float value, modifier_bitset_type const& modifiers ) -> input_value_type
    {
        // TODO: optimize the modifier application
        if ( modifiers.test( bit_cast( modifier::negate ) ) )
        {
            value = -1 * value;
        }

        if ( modifiers.test( bit_cast( modifier::swizzle ) ) )
        {
            return glm::vec2{ 0.F, value };
        }
        return value;
    }


    auto merge_value_to_snapshot( input_reduction& snapshot, input_value_type const value ) -> void
    {
        // TODO: improve this with a priority system.
        std::visit( [&]<typename TStart, typename TToAdd>( TStart&& start, TToAdd&& to_add )
                        {
                            using start_type  = std::decay_t<TStart>;
                            using to_add_type = std::decay_t<TToAdd>;

                            // we make sure that vec2 remains the relevant type.
                            if constexpr ( std::is_same_v<to_add_type, glm::vec2> )
                            {
                                // please change this it's horrible
                                snapshot.value = convert_input_value<glm::vec2>( start ) + to_add;
                            }
                            // if dealing with a base boolean, we convert to bool instead
                            else if constexpr ( std::is_same_v<start_type, bool> )
                            {
                                snapshot.value = static_cast<float>( start ) + convert_input_value<float>( to_add );
                            }
                            else
                            {
                                start += convert_input_value<start_type>( to_add );
                            }
                        }, snapshot.value, value );
    }

}
