#ifndef RST_ECS_VIEW_H
#define RST_ECS_VIEW_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/meta/type_traits.h>
#include <rst/__core/__ecs/component_constraints.h>
#include <rst/__core/__ecs/ecs_error.h>
#include <rst/__core/__ecs/entity.h>
#include <rst/__core/__ecs/registry_pool.h>


namespace rst::ecs
{
    namespace internal
    {
        /**
         * @brief Iterator for view ranges that yields entity-component tuples.
         *
         * This iterator provides forward iteration over entities that possess all required
         * component types, yielding tuples containing the entity ID and references to
         * its components. It's designed to work with structured bindings for clean syntax.
         *
         * @code
         * // usage with structured bindings
         * auto view = registry.view<position, velocity>();
         * 
         * for (auto [entity, pos, vel] : view.each()) {
         *     // entity is entity_type
         *     // pos is position& (mutable reference)
         *     // vel is velocity& (mutable reference)
         *     
         *     pos.x += vel.dx;
         *     pos.y += vel.dy;
         * }
         * 
         * // traditional iterator usage
         * auto range = view.each();
         * for (auto it = range.begin(); it != range.end(); ++it) {
         *     auto [entity, pos, vel] = *it;
         *     // process components...
         * }
         * @endcode
         *
         * @note Iterator stability: This iterator has specific invalidation behavior due
         * to sparse set implementation details. Operations that affect iterator validity:
         * - insert() - safe during iteration (appends to end, doesn't affect current positions)
         * - remove() - may cause iteration anomalies (swap-and-pop can move unvisited elements)
         * - clear() - fully invalidates the iterator
         * - reserve() - safe (only pre-allocates, doesn't move existing elements)
         * - Modifying component data through the yielded references is always safe
         *
         * @tparam TView The view type this iterator operates on
         * @tparam TComponents The component types being iterated over
         */
        template <typename TView, detail::viewable_ecs_component... TComponents> requires ( sizeof...( TComponents ) > 0U )
        class view_range_iterator
        {
        public:
            using value_type = std::conditional_t<std::is_const_v<TView>,
                std::tuple<entity_type, TComponents const&...>, std::tuple<entity_type, TComponents&...>>;

            using underlying_iterator_type = sparse_intersection_iterator<entity_type, TComponents...>;


            view_range_iterator( ) = default;


            explicit view_range_iterator( TView& view, underlying_iterator_type it )
                : view_ref_{ view }
                , it_{ it } { }


            [[nodiscard]] auto operator*( ) const -> value_type
            {
                entity_type const entity = *it_;
                return std::tuple_cat(
                    std::forward_as_tuple( entity ),
                    meta::wrap_unique( view_ref_.template get<TComponents...>( entity ).value( ) ) );
            }


            auto operator++( ) -> view_range_iterator&
            {
                ++it_;
                return *this;
            }


            auto operator++( int ) -> view_range_iterator
            {
                auto temp = *this;
                ++it_;
                return temp;
            }


            [[nodiscard]] auto operator==( view_range_iterator const& other ) const noexcept -> bool { return it_ == other.it_; }
            [[nodiscard]] auto operator!=( view_range_iterator const& other ) const noexcept -> bool { return it_ != other.it_; }

        private:
            TView& view_ref_;
            underlying_iterator_type it_;
        };
    }


    /**
     * @brief A view over entities that possess all specified components.
     *
     * The view provides efficient iteration over entities in an ECS registry that have all
     * the required component types. It uses sparse set intersection to minimize iteration
     * overhead by starting with the smallest component pool.
     *
     * Key features:
     * - Lazy evaluation - entities are checked during iteration.
     * - Cache-friendly iteration using packed arrays.
     * - Multiple iteration patterns via each() overloads.
     * - Compile-time type safety with subset component access.
     * - Support for both mutable and const access patterns.
     * - Range-based for loop support with structured bindings.
     *
     * @code
     * // basic view usage for system processing
     * auto view = registry.view<position, velocity>();
     * 
     * // method 1: lambda-based iteration with entity ID
     * view.each([](entity_type entity, position& pos, velocity& vel) {
     *     pos.x += vel.dx;
     *     pos.y += vel.dy;
     *     // process entity with both position and velocity
     * });
     * 
     * // method 2: lambda-based iteration without entity ID
     * view.each([](position& pos, velocity& vel) {
     *     pos.x += vel.dx;
     *     pos.y += vel.dy;
     * });
     * 
     * // method 3: range-based for loop with structured bindings
     * for (auto [entity, pos, vel] : view.each()) {
     *     pos.x += vel.dx;
     *     pos.y += vel.dy;
     *     // entity is the entity ID, pos and vel are component references
     * }
     * 
     * // method 4: traditional iterator-based iteration
     * for (auto it = view.begin(); it != view.end(); ++it) {
     *     entity_type entity = *it;
     *     auto [pos, vel] = view.get<position, velocity>(entity).value();
     *     pos.x += vel.dx;
     *     pos.y += vel.dy;
     * }
     * 
     * // component subset access (get only position from position+velocity view)
     * for (entity_type entity : view) {
     *     auto pos_result = view.get<position>(entity);
     *     if (pos_result) {
     *         position& pos = pos_result.value();
     *         // process only position component
     *     }
     * }
     * 
     * // check if specific entity has components in this view
     * if (view.has<position, velocity>(some_entity)) {
     *     // entity has both components in this view
     * }
     * @endcode
     *
     * @tparam TComponents The component types that entities must possess.
     */
    template <detail::viewable_ecs_component... TComponents> requires ( sizeof...( TComponents ) > 0U )
    class view final
    {
    public:
        using iterator_type       = sparse_intersection_iterator<entity_type, TComponents...>;
        using const_iterator_type = sparse_intersection_iterator<entity_type, TComponents...>;

        template <typename... UComponents>
        using underlying_view_get_type =
        meta::unwrap_single<typename meta::fallback<std::tuple, UComponents&...>::template if_empty<TComponents&...>::type>::type;

        /**
         * The return type of get() based on requested component types.
         * - If no types are specified, returns a tuple of all component references.
         * - If a single type is specified, returns a direct reference to that component.
         * - If multiple types are specified, returns a tuple of those component references.
         * @tparam UComponents
         */
        template <typename... UComponents>
        using view_get_type = std::expected<underlying_view_get_type<UComponents...>, ecs_error>;


        template <typename TView>
        class view_range final
        {
        public:
            using iterator_type       = internal::view_range_iterator<TView, TComponents...>;
            using const_iterator_type = internal::view_range_iterator<TView, TComponents...>;


            explicit view_range( TView& view ) noexcept : view_ref_{ view } { }


            [[nodiscard]] auto begin( ) noexcept -> iterator_type requires ( not std::is_const_v<TView> )
            {
                return iterator_type{ view_ref_, view_ref_.begin( ) };
            }


            [[nodiscard]] auto end( ) noexcept -> iterator_type requires ( not std::is_const_v<TView> )
            {
                return iterator_type{ view_ref_, view_ref_.end( ) };
            }


            [[nodiscard]] auto begin( ) const noexcept -> const_iterator_type
            {
                return const_iterator_type{ view_ref_, view_ref_.begin( ) };
            }


            [[nodiscard]] auto end( ) const noexcept -> const_iterator_type
            {
                return const_iterator_type{ view_ref_, view_ref_.end( ) };
            }

        private:
            TView& view_ref_;
        };


        /**
         * @brief Constructs a view from component pools.
         *
         * @param pools Tuple of component pool references for the required component types.
         *
         * @complexity O(1)
         */
        explicit view( detail::reg_pool_type<TComponents>&... pools ) noexcept
            : pools_{ pools... }
            , smallest_pool_ref_{ *meta::find_smallest<base_sparse_set<entity_type>>( pools_ ) } { }


        /**
         * @brief Checks if an entity has specified components.
         *
         * @tparam UComponents Component types to check (must be a decayed subset of TComponents).
         * @param entity The entity to check.
         * @return True if entity has all specified components, false otherwise.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::contains_type<TComponents...>... UComponents>
        [[nodiscard]] auto has( entity_type const entity ) const noexcept -> bool
        {
            return ( std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).has( entity ) && ... );
        }


        /**
         * @brief Retrieves mutable references wrapped std::expected to entity's components.
         *
         * @tparam UComponents Component types to retrieve (must be a decayed subset of TComponents).
         * @param entity The entity to get components from.
         * @return One of the following wrapped around std::expected based on UComponents:
         * - If no template parameters, reference to all component pools.
         * - If single component, direct reference to that component.
         * - If multiple components, tuple of component references.
         * - If error, ecs_error indicating the failure reason.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <typename... UComponents> requires ( meta::contains_type<UComponents, TComponents...> && ... )
        [[nodiscard]] auto get( entity_type const entity ) noexcept -> view_get_type<UComponents...>
        {
            return get_impl<view_get_type<UComponents...>, UComponents...>( entity );
        }


        /**
         * @brief Retrieves mutable references to entity's components. Will assert on error.
         *
         * @tparam UComponents Component types to retrieve (must be a decayed subset of TComponents).
         * @param entity The entity to get components from.
         * @return One of the following wrapped around std::expected based on UComponents:
         * - If no template parameters, reference to all component pools.
         * - If single component, direct reference to that component.
         * - If multiple components, tuple of component references.
         * - If error, ecs_error indicating the failure reason.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::contains_type<TComponents...>... UComponents>
        [[nodiscard]] auto get( entity_type const entity ) const noexcept -> view_get_type<UComponents const...>
        {
            return get_impl<view_get_type<UComponents const...>, UComponents...>( entity );
        }


        /**
         * @brief Retrieves const references to entity's components. Will assert on error.
         *
         * @tparam UComponents Component types to retrieve (must be a decayed subset of TComponents).
         * @param entity The entity to get components from.
         * @return One of the following based on UComponents:
         * - If no template parameters, reference to all component pools.
         * - If single component, direct reference to that component.
         * - If multiple components, tuple of component references.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::contains_type<TComponents...>... UComponents>
        [[nodiscard]] auto unsafe_get( entity_type const entity ) noexcept -> underlying_view_get_type<UComponents...>
        {
            assert( has<UComponents...>( entity ) && "view::unsafe_get: entity not found!" );
            return get_impl<underlying_view_get_type<UComponents...>, UComponents...>( entity );
        }


        /**
         * @brief Retrieves const references to entity's components.
         *
         * @tparam UComponents Component types to retrieve (must be a decayed subset of TComponents).
         * @param entity The entity to get components from.
         * @return One of the following based on UComponents:
         * - If no template parameters, reference to all component pools.
         * - If single component, direct reference to that component.
         * - If multiple components, tuple of component references.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::contains_type<TComponents...>... UComponents>
        [[nodiscard]] auto unsafe_get( entity_type const entity ) const noexcept -> underlying_view_get_type<UComponents const...>
        {
            assert( has<UComponents...>( entity ) && "view::unsafe_get: entity not found!" );
            return get_impl<underlying_view_get_type<UComponents const...>, UComponents...>( entity );
        }


        /**
         * @brief Applies a function to each entity with entity ID and mutable component references.
         *
         * @tparam TDelegate Function type that accepts (entity_type, TComponents&...).
         * @param delegate Function to apply to each entity and its components.
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components.
         */
        template <std::invocable<entity_type, TComponents&...> TDelegate>
        auto each( TDelegate&& delegate ) noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            each_impl<true>( std::forward<TDelegate>( delegate ) );
        }


        /**
         * @brief Applies a function to each entity with mutable component references only.
         *
         * @tparam TDelegate Function type that accepts (TComponents&...).
         * @param delegate Function to apply to each entity's components.
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components.
         */
        template <std::invocable<TComponents&...> TDelegate>
        auto each( TDelegate&& delegate ) noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            each_impl<false>( std::forward<TDelegate>( delegate ) );
        }


        /**
         * @brief Applies a function to each entity with entity ID and const component references.
         *
         * @tparam TDelegate Function type that accepts (entity_type, TComponents const&...).
         * @param delegate Function to apply to each entity and its components.
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components.
         */
        template <std::invocable<entity_type, TComponents const&...> TDelegate>
        auto each( TDelegate&& delegate ) const noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            each_impl<true>( std::forward<TDelegate>( delegate ) );
        }


        /**
         * @brief Applies a function to each entity with const component references only.
         *
         * @tparam TDelegate Function type that accepts (TComponents const&...).
         * @param delegate Function to apply to each entity's components.
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components.
         */
        template <std::invocable<TComponents const&...> TDelegate>
        auto each( TDelegate&& delegate ) const noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            each_impl<false>( std::forward<TDelegate>( delegate ) );
        }


        /**
         * @brief Returns a view_range for structured binding iteration.
         *
         * @return A view_range that provides iterators over entity-component tuples.
         *
         * @complexity O(1) - creates lightweight wrapper
         */
        auto each( ) noexcept -> view_range<view>
        {
            return view_range<view>{ *this };
        }


        /**
         * @brief Returns a const view_range for structured binding iteration.
         *
         * @return A const view_range that provides iterators over entity-component tuples.
         *
         * @complexity O(1) - creates lightweight wrapper
         */
        auto each( ) const noexcept -> view_range<view const>
        {
            return view_range<view const>{ *this };
        }


        // +--------------------------------+
        // | ITERATOR INTERFACE             |
        // +--------------------------------+
        /**
         * @brief Returns an iterator to the beginning of entities with all required components.
         *
         * @return Iterator pointing to the first entity in the view.
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto begin( ) noexcept(std::is_nothrow_constructible_v<iterator_type>) -> iterator_type
        {
            return std::apply(
                [this]( auto&... pools ) { return iterator_type::begin( smallest_pool_ref_, pools... ); }, pools_ );
        }


        /**
         * @brief Returns an iterator to the end of entities with all required components.
         *
         * @return Iterator pointing past the last entity in the view.
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto end( ) noexcept(std::is_nothrow_constructible_v<iterator_type>) -> iterator_type
        {
            return std::apply(
                [this]( auto&... pools ) { return iterator_type::end( smallest_pool_ref_, pools... ); }, pools_ );
        }


        /**
         * @brief Returns a const iterator to the beginning of entities with all required components.
         *
         * @return Const iterator pointing to the first entity in the view.
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto begin( ) const noexcept(std::is_nothrow_invocable_v<const_iterator_type>) -> const_iterator_type
        {
            return std::apply(
                [this]( auto const&... pools ) { return const_iterator_type::begin( smallest_pool_ref_, pools... ); }, pools_ );
        }


        /**
         * @brief Returns a const iterator to the end of entities with all required components.
         *
         * @return Const iterator pointing past the last entity in the view.
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto end( ) const noexcept(std::is_nothrow_constructible_v<const_iterator_type>) -> const_iterator_type
        {
            return std::apply(
                [this]( auto const&... pools ) { return const_iterator_type::end( smallest_pool_ref_, pools... ); }, pools_ );
        }

    private:
        std::tuple<detail::reg_pool_type<TComponents>&...> const pools_;
        detail::base_reg_pool_type& smallest_pool_ref_;


        template <typename TResult, meta::contains_type<TComponents...>... UComponents>
        [[nodiscard]] auto get_impl( entity_type const entity ) const noexcept -> TResult
        {
            // if the requested components are empty, recurse to get all components
            if constexpr ( sizeof...( UComponents ) == 0U )
            {
                return get_impl<TResult, TComponents...>( entity );
            }
            else
            {
                // add validation logic if TResult expects it
                if constexpr ( meta::expected_like<TResult, ecs_error> )
                {
                    // ... validation logic
                    // we don't need to check for component existence, as that is enforced by the "requires" clause.
                    if ( entity == null_entity )
                    {
                        return std::unexpected{ ecs_error::invalid_entity_id };
                    }
                    if ( not smallest_pool_ref_.has( entity ) )
                    {
                        return std::unexpected{ ecs_error::entity_not_found };
                    }
                }

                // return the requested components, wrapped around a std::expected
                if constexpr ( meta::expected_like<TResult, ecs_error> )
                {
                    return TResult{
                        std::in_place, std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).unsafe_get( entity )...
                    };
                }
                else
                {
                    return TResult{ std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).unsafe_get( entity )... };
                }
            }
        }


        template <bool include_entity, typename TDelegate>
        auto each_impl( TDelegate&& delegate ) const noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            for ( entity_type entity : *this )
            {
                // we give for granted this entity is valid, as it comes from the smallest pool iteration
                if constexpr ( include_entity )
                {
                    delegate( entity, std::get<meta::index_of_v<TComponents, TComponents...>>( pools_ ).unsafe_get( entity )... );
                }
                else
                {
                    delegate( std::get<meta::index_of_v<TComponents, TComponents...>>( pools_ ).unsafe_get( entity )... );
                }
            }
        }
    };
}


#endif //!RST_ECS_VIEW_H
