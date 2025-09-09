#ifndef RST_ECS_VIEW_H
#define RST_ECS_VIEW_H

#include <rst/pch.h>

#include <rst/meta/reference.h>
#include <rst/meta/tuple.h>


namespace rst::ecs
{
    namespace internal
    {
        /**
         * Mutable iterator for intersecting a view's components.
         * @tparam TComponents
         */
        template <typename... TComponents>
        using view_iterator_type = sparse_intersection_iterator<entity_type, TComponents...>;

        /**
         * Const iterator for intersecting a view's components.
         * @note Same as non-const since the iterator does not allow mutation in the first place.
         * @tparam TComponents
         */
        template <typename... TComponents>
        using view_const_iterator_type = sparse_intersection_iterator<entity_type, TComponents...>;


        template <typename TView, meta::non_reference... TComponents> requires ( sizeof...( TComponents ) > 0U )
        class view_range_iterator
        {
        public:
            using iterator_category_type = std::forward_iterator_tag;
            using difference_type        = std::ptrdiff_t;

            using value_type = std::tuple<entity_type, TComponents&...>;

            using underlying_iterator_type = sparse_intersection_iterator<entity_type, TComponents...>;


            view_range_iterator( ) = default;


            explicit view_range_iterator( TView& view, underlying_iterator_type it )
                : view_ref_{ view }
                , it_{ it } { }


            [[nodiscard]] auto operator*( ) const -> value_type
            {
                entity_type const entity = *it_;
                return std::tuple_cat( std::make_tuple( entity ), view_ref_.template get<TComponents...>( entity ) );
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
     * • Lazy evaluation - entities are checked during iteration
     * • Cache-friendly iteration using packed arrays
     * • Multiple iteration patterns via each() overloads
     * • Compile-time type safety with subset component access
     * • Support for both mutable and const access patterns
     *
     * @tparam TComponents The component types that entities must possess.
     */
    template <meta::non_reference... TComponents> requires ( sizeof...( TComponents ) > 0U )
    class view final
    {
    public:
        using component_pools_type = std::tuple<internal::component_pool_type<TComponents>&...>;
        using ec_type              = std::tuple<entity_type, TComponents&...>;

        using iterator_type       = internal::view_iterator_type<TComponents...>;
        using const_iterator_type = internal::view_const_iterator_type<TComponents...>;


        class view_range final
        {
        public:
            using iterator_type = internal::view_range_iterator<view, TComponents...>;

            explicit view_range( view& view ) noexcept : view_ref_{ view } { }


            [[nodiscard]] auto begin( ) const noexcept -> iterator_type
            {
                return iterator_type{ view_ref_, view_ref_.begin( ) };
            }


            [[nodiscard]] auto end( ) const noexcept -> iterator_type
            {
                return iterator_type{ view_ref_, view_ref_.end( ) };
            }

        private:
            view& view_ref_;
        };


        /**
         * @brief Constructs a view from component pools.
         *
         * @param pools Tuple of component pool references for the required component types.
         *
         * @complexity O(1)
         */
        explicit view( component_pools_type const& pools ) noexcept : pools_{ pools } { }


        /**
         * @brief Checks if an entity has specified components.
         *
         * @tparam UComponents Component types to check (must be subset of TComponents).
         * @param entity The entity to check.
         * @return True if entity has all specified components, false otherwise.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::non_reference... UComponents> requires ( meta::contains_type<UComponents, TComponents...> && ... )
        [[nodiscard]] auto has( entity_type const entity ) const noexcept -> bool
        {
            return ( std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).has( entity ) && ... );
        }


        /**
         * @brief Retrieves mutable references to entity's components.
         *
         * @tparam UComponents Component types to retrieve (must be subset of TComponents).
         * @param entity The entity to get components from.
         * @return Component reference(s) - single reference if one component, tuple if multiple.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::non_reference... UComponents> requires ( meta::contains_type<UComponents, TComponents...> && ... )
        [[nodiscard]] auto get( entity_type const entity ) noexcept -> meta::unwrap_single_t<std::tuple<UComponents&...>>
        {
            return { std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).get( entity )... };
        }


        /**
         * @brief Retrieves const references to entity's components.
         *
         * @tparam UComponents Component types to retrieve (must be subset of TComponents).
         * @param entity The entity to get components from.
         * @return Const component reference(s) - single reference if one component, tuple if multiple.
         *
         * @complexity O(k) where k is the number of UComponents.
         * @note UComponents must be a subset of TComponents.
         */
        template <meta::non_reference... UComponents> requires ( meta::contains_type<UComponents, TComponents...> && ... )
        [[nodiscard]] auto get(
            entity_type const entity ) const noexcept -> meta::unwrap_single_t<std::tuple<UComponents const&...>>
        {
            return { std::get<meta::index_of_v<UComponents, TComponents...>>( pools_ ).get( entity )... };
        }


        /**
         * @brief Applies a function to each entity with entity ID and mutable component references.
         *
         * @tparam TDelegate Function type that accepts (entity_type, TComponents&...)
         * @param delegate Function to apply to each entity and its components
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components
         */
        template <std::invocable<entity_type, TComponents&...> TDelegate>
        auto each( TDelegate&& delegate ) noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            for ( entity_type entity : *this )
            {
                std::apply(
                    delegate, std::tuple_cat( std::make_tuple( entity ), meta::wrap_unique( get<TComponents...>( entity ) ) ) );
            }
        }


        /**
         * @brief Applies a function to each entity with mutable component references only.
         *
         * @tparam TDelegate Function type that accepts (TComponents&...)
         * @param delegate Function to apply to each entity's components
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components
         */
        template <std::invocable<TComponents&...> TDelegate>
        auto each( TDelegate&& delegate ) noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            for ( entity_type entity : *this )
            {
                std::apply( delegate, meta::wrap_unique( get<TComponents...>( entity ) ) );
            }
        }


        /**
         * @brief Applies a function to each entity with entity ID and const component references.
         *
         * @tparam TDelegate Function type that accepts (entity_type, TComponents const&...)
         * @param delegate Function to apply to each entity and its components
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components
         */
        template <std::invocable<entity_type, TComponents const&...> TDelegate>
        auto each( TDelegate&& delegate ) const noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            for ( entity_type entity : *this )
            {
                std::apply(
                    delegate, std::tuple_cat( std::make_tuple( entity ), meta::wrap_unique( get<TComponents...>( entity ) ) ) );
            }
        }


        /**
         * @brief Applies a function to each entity with const component references only.
         *
         * @tparam TDelegate Function type that accepts (TComponents const&...)
         * @param delegate Function to apply to each entity's components
         *
         * @complexity O(n×k) where n is the number of matching entities, k is the number of components
         */
        template <std::invocable<TComponents const&...> TDelegate>
        auto each( TDelegate&& delegate ) const noexcept(std::is_nothrow_invocable_v<TDelegate>) -> void
        {
            for ( entity_type entity : *this )
            {
                std::apply( delegate, meta::wrap_unique( get<TComponents...>( entity ) ) );
            }
        }


        /**
         * @brief Returns a view_range for structured binding iteration.
         *
         * @return A view_range that provides iterators over entity-component tuples
         *
         * @complexity O(1) - creates lightweight wrapper
         */
        auto each( ) noexcept -> view_range
        {
            return view_range{ *this };
        }


        // +--------------------------------+
        // | ITERATOR INTERFACE             |
        // +--------------------------------+
        /**
         * @brief Returns an iterator to the beginning of entities with all required components.
         *
         * @return Iterator pointing to the first entity in the view
         *
         * @complexity O(k) where k is the number of component types (to find the smallest pool)
         */
        [[nodiscard]] auto begin( ) noexcept(std::is_nothrow_constructible_v<iterator_type>) -> iterator_type
        {
            return std::apply( []( auto&... pools ) { return iterator_type::begin( pools... ); }, pools_ );
        }


        /**
         * @brief Returns an iterator to the end of entities with all required components.
         *
         * @return Iterator pointing past the last entity in the view
         *
         * @complexity O(k) where k is the number of component types (to find the smallest pool)
         */
        [[nodiscard]] auto end( ) noexcept(std::is_nothrow_constructible_v<iterator_type>) -> iterator_type
        {
            return std::apply( []( auto&... pools ) { return iterator_type::end( pools... ); }, pools_ );
        }


        /**
         * @brief Returns a const iterator to the beginning of entities with all required components.
         *
         * @return Const iterator pointing to the first entity in the view
         *
         * @complexity O(k) where k is the number of component types (to find the smallest pool)
         */
        [[nodiscard]] auto begin( ) const noexcept(std::is_nothrow_invocable_v<const_iterator_type>) -> const_iterator_type
        {
            return std::apply( []( auto&... pools ) { return const_iterator_type::begin( pools... ); }, pools_ );
        }


        /**
         * @brief Returns a const iterator to the end of entities with all required components.
         *
         * @return Const iterator pointing past the last entity in the view
         *
         * @complexity O(k) where k is the number of component types (to find the smallest pool)
         */
        [[nodiscard]] auto end( ) const noexcept(std::is_nothrow_constructible_v<const_iterator_type>) -> const_iterator_type
        {
            return std::apply( []( auto&... pools ) { return const_iterator_type::end( pools... ); }, pools_ );
        }

    private:
        component_pools_type pools_;
    };
}


#endif //!RST_ECS_VIEW_H
