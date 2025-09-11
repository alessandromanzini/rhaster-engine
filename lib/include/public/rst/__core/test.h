// +--------------------------------+
// | SOA-STYLE ITERATION            |
// +--------------------------------+
// /**
//  * @brief Process components in Structure-of-Arrays style for optimal cache performance.
//  *
//  * This method processes all components of the same type together, improving cache locality
//  * and enabling potential vectorization. Instead of accessing [entity1.pos, entity1.vel,
//  * entity2.pos, entity2.vel], it processes [entity1.pos, entity2.pos, ...] then
//  * [entity1.vel, entity2.vel, ...].
//  *
//  * @code
//  * auto view = registry.view<position, velocity>();
//  *
//  * // soa-style processing for maximum cache efficiency
//  * view.each_soa([](std::span<entity_type const> entities,
//  *                  std::span<position> positions,
//  *                  std::span<velocity> velocities) {
//  *     // process all positions together (vectorizable)
//  *     for (size_t i = 0; i < entities.size(); ++i) {
//  *         positions[i].x += velocities[i].dx;
//  *         positions[i].y += velocities[i].dy;
//  *     }
//  * });
//  *
//  * // alternative: process individual component arrays
//  * view.each_component<position>([](entity_type entity, position& pos) {
//  *     // only processes position components
//  * });
//  * @endcode
//  *
//  * @tparam TDelegate Function type that accepts (std::span<entity_type const>, std::span<TComponents>...)
//  * @param delegate Function to apply to component arrays
//  *
//  * @complexity O(n×k) where n is the number of matching entities, k is the number of components
//  * @note This method pre-allocates temporary arrays for optimal cache access patterns
//  */
// template <std::invocable<std::span<entity_type const>, std::span<TComponents>...> TDelegate>
// auto each_soa( TDelegate&& delegate ) -> void
// {
//     // pre-allocate arrays for cache-friendly access
//     std::vector<entity_type> entity_batch{};
//     std::tuple<std::vector<std::reference_wrapper<TComponents>>...> component_batches{};
//
//     // reserve space to avoid reallocations
//     auto const estimated_size = smallest_pool_ref_.size();
//     entity_batch.reserve( estimated_size );
//     ( std::get<std::vector<std::reference_wrapper<TComponents>>>( component_batches ).reserve( estimated_size ), ... );
//
//     // collect all entities and their components
//     for ( entity_type entity : *this )
//     {
//         entity_batch.push_back( entity );
//         // collect components into separate arrays
//         ( std::get<std::vector<std::reference_wrapper<TComponents>>>( component_batches )
//             .emplace_back( std::get<meta::index_of_v<TComponents, TComponents...>>( pools_ ).get( entity ) ), ... );
//     }
//
//     // convert to spans for the delegate
//     auto entity_span = std::span<entity_type const>{ entity_batch };
//     auto component_spans = std::make_tuple(
//         std::span<TComponents>{
//             reinterpret_cast<TComponents*>( std::get<std::vector<std::reference_wrapper<TComponents>>>( component_batches ).data() ),
//             std::get<std::vector<std::reference_wrapper<TComponents>>>( component_batches ).size()
//         }...
//     );
//
//     // call delegate with soa-style arrays
//     std::apply( [&delegate, entity_span]( auto... spans ) {
//         delegate( entity_span, spans... );
//     }, component_spans );
// }
//
// /**
//  * @brief Process components in Structure-of-Arrays style with const access.
//  *
//  * Const version of each_soa for read-only component processing.
//  *
//  * @tparam TDelegate Function type that accepts (std::span<entity_type const>, std::span<TComponents const>...)
//  * @param delegate Function to apply to component arrays
//  *
//  * @complexity O(n×k) where n is the number of matching entities, k is the number of components
//  */
// template <std::invocable<std::span<entity_type const>, std::span<TComponents const>...> TDelegate>
// auto each_soa( TDelegate&& delegate ) const -> void
// {
//     // pre-allocate arrays for cache-friendly access
//     std::vector<entity_type> entity_batch{};
//     std::tuple<std::vector<std::reference_wrapper<TComponents const>>...> component_batches{};
//
//     // reserve space to avoid reallocations
//     auto const estimated_size = smallest_pool_ref_.size();
//     entity_batch.reserve( estimated_size );
//     ( std::get<std::vector<std::reference_wrapper<TComponents const>>>( component_batches ).reserve( estimated_size ), ... );
//
//     // collect all entities and their components
//     for ( entity_type entity : *this )
//     {
//         entity_batch.push_back( entity );
//         // collect components into separate arrays
//         ( std::get<std::vector<std::reference_wrapper<TComponents const>>>( component_batches )
//             .emplace_back( std::get<meta::index_of_v<TComponents, TComponents...>>( pools_ ).get( entity ) ), ... );
//     }
//
//     // convert to spans for the delegate
//     auto entity_span = std::span<entity_type const>{ entity_batch };
//     auto component_spans = std::make_tuple(
//         std::span<TComponents const>{
//             reinterpret_cast<TComponents const*>( std::get<std::vector<std::reference_wrapper<TComponents const>>>( component_batches ).data() ),
//             std::get<std::vector<std::reference_wrapper<TComponents const>>>( component_batches ).size()
//         }...
//     );
//
//     // call delegate with soa-style arrays
//     std::apply( [&delegate, entity_span]( auto... spans ) {
//         delegate( entity_span, spans... );
//     }, component_spans );
// }
//
// /**
//  * @brief Process a single component type across all matching entities.
//  *
//  * Processes only one component type in a tight loop, maximizing cache efficiency
//  * and enabling compiler auto-vectorization for simple operations.
//  *
//  * @code
//  * auto view = registry.view<position, velocity>();
//  *
//  * // process only positions in a tight, vectorizable loop
//  * view.each_component<position>([](entity_type entity, position& pos) {
//  *     pos.x *= 0.99f; // damping - highly vectorizable
//  *     pos.y *= 0.99f;
//  * });
//  *
//  * // process without entity ID for maximum performance
//  * view.each_component<velocity>([](velocity& vel) {
//  *     vel.dx = std::clamp(vel.dx, -100.0f, 100.0f); // vectorizable clamp
//  *     vel.dy = std::clamp(vel.dy, -100.0f, 100.0f);
//  * });
//  * @endcode
//  *
//  * @tparam TComponent The component type to process (must be in TComponents)
//  * @tparam TDelegate Function type that accepts (entity_type, TComponent&) or (TComponent&)
//  * @param delegate Function to apply to each component
//  *
//  * @complexity O(n) where n is the number of matching entities
//  * @note This is the fastest iteration method for single-component processing
//  */
// template <typename TComponent, typename TDelegate>
// requires ( meta::contains_type<TComponent, TComponents...> &&
//            ( std::invocable<TDelegate, entity_type, TComponent&> || std::invocable<TDelegate, TComponent&> ) )
// auto each_component( TDelegate&& delegate ) -> void
// {
//     auto& component_pool = std::get<meta::index_of_v<TComponent, TComponents...>>( pools_ );
//     auto const& packed_entities = component_pool.packed();
//     auto components = component_pool.data();
//
//     if constexpr ( std::invocable<TDelegate, entity_type, TComponent&> )
//     {
//         // include entity ID in delegate call
//         for ( std::size_t i = 0; i < packed_entities.size(); ++i )
//         {
//             if ( has( packed_entities[i] ) ) // ensure entity has all required components
//             {
//                 delegate( packed_entities[i], components[i] );
//             }
//         }
//     }
//     else
//     {
//         // component-only processing for maximum performance
//         for ( std::size_t i = 0; i < packed_entities.size(); ++i )
//         {
//             if ( has( packed_entities[i] ) ) // ensure entity has all required components
//             {
//                 delegate( components[i] );
//             }
//         }
//     }
// }
//
// /**
//  * @brief Process a single component type with const access.
//  *
//  * Const version of each_component for read-only component processing.
//  *
//  * @tparam TComponent The component type to process (must be in TComponents)
//  * @tparam TDelegate Function type that accepts (entity_type, TComponent const&) or (TComponent const&)
//  * @param delegate Function to apply to each component
//  *
//  * @complexity O(n) where n is the number of matching entities
//  */
// template <typename TComponent, typename TDelegate>
// requires ( meta::contains_type<TComponent, TComponents...> &&
//            ( std::invocable<TDelegate, entity_type, TComponent const&> || std::invocable<TDelegate, TComponent const&> ) )
// auto each_component( TDelegate&& delegate ) const -> void
// {
//     auto const& component_pool = std::get<meta::index_of_v<TComponent, TComponents...>>( pools_ );
//     auto const& packed_entities = component_pool.packed();
//     auto const components = component_pool.data();
//
//     if constexpr ( std::invocable<TDelegate, entity_type, TComponent const&> )
//     {
//         // include entity ID in delegate call
//         for ( std::size_t i = 0; i < packed_entities.size(); ++i )
//         {
//             if ( has( packed_entities[i] ) ) // ensure entity has all required components
//             {
//                 delegate( packed_entities[i], components[i] );
//             }
//         }
//     }
//     else
//     {
//         // component-only processing for maximum performance
//         for ( std::size_t i = 0; i < packed_entities.size(); ++i )
//         {
//             if ( has( packed_entities[i] ) ) // ensure entity has all required components
//             {
//                 delegate( components[i] );
//             }
//         }
//     }
// }