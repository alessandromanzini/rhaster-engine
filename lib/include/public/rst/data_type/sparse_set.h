#ifndef RST_SPARSE_SET_H
#define RST_SPARSE_SET_H

#include <rst/pch.h>

#include <rst/data_type/data_structure_error.h>
#include <rst/data_type/ref_proxy.h>
#include <rst/meta/algorithm.h>


// TODO: add documentation and move to appropriate folder
namespace rst
{
    // +--------------------------------+
    // | BASE SPARSE SET CLASS          |
    // +--------------------------------+
    template <std::unsigned_integral TIndex>
    class base_sparse_set
    {
    public:
        using index_type        = TIndex;
        using sparse_index_type = TIndex;


        base_sparse_set( ) noexcept          = default;
        virtual ~base_sparse_set( ) noexcept = default;

        base_sparse_set( base_sparse_set const& )                        = delete;
        base_sparse_set( base_sparse_set&& ) noexcept                    = delete;
        auto operator=( base_sparse_set const& ) -> base_sparse_set&     = delete;
        auto operator=( base_sparse_set&& ) noexcept -> base_sparse_set& = delete;

        [[nodiscard]] virtual auto has( index_type index ) const noexcept -> bool = 0;
        virtual auto remove( index_type index ) -> void = 0;
        virtual auto clear( ) -> void = 0;

        [[nodiscard]] virtual auto packed( ) const noexcept -> std::span<index_type const> = 0;
        [[nodiscard]] virtual auto size( ) const noexcept -> std::size_t = 0;
        [[nodiscard]] virtual auto capacity( ) const noexcept -> std::size_t = 0;
        [[nodiscard]] virtual auto empty( ) const noexcept -> bool = 0;
    };


    // +--------------------------------+
    // | SPARSE SET TEMPLATED IMPL      |
    // +--------------------------------+
    /**
     * @brief A sparse set data structure providing O(1) operations for component storage.
     *
     * Sparse sets maintain two arrays: a sparse array for O(1) lookups and a packed
     * array for cache-friendly iteration. Ideal for ECS component storage where entity
     * IDs may be sparse but component iteration should be packed.
     *
     * Key features:
     * - O(1) insertion, removal, and lookup operations.
     * - Cache-friendly packed iteration over elements.
     * - Stable indices during insertion (existing elements don't move).
     * - Swap-and-pop removal maintains packed storage efficiency.
     * - Support for both const and mutable element types.
     *
     * @code
     * // basic usage example
     * sparse_set<int> components{};
     * 
     * // insert components for entities 5, 100, 42
     * components.insert(5, 10);
     * components.insert(100, 20);
     * components.insert(42, 30);
     * 
     * // fast lookup and access
     * if (components.has(100)) {
     *     int& value = components.get(100); // O(1) access
     *     value = 25;
     * }
     * 
     * // cache-friendly iteration over packed elements
     * for (auto const& element : components.data()) {
     *     // process elements in packed order
     * }
     * 
     * // iterate over indices to get entity IDs
     * for (auto const index : components.packed()) {
     *     auto& element = components.get(index);
     *     // process entity 'index' with its 'element'
     * }
     * 
     * // remove entity 100's component
     * components.remove(100); // O(1) removal
     * @endcode
     *
     * @tparam TElement The type of elements stored (must be default constructible and move assignable).
     * @tparam TIndex The index type (must be unsigned integral, defaults to uint32_t).
     */
    template <typename TElement, std::unsigned_integral TIndex = uint32_t>
        requires ( std::is_default_constructible_v<TElement> && not std::is_reference_v<TElement> &&
                   std::is_move_assignable_v<std::remove_cv_t<TElement>> )
    class sparse_set final : public base_sparse_set<TIndex>
    {
    public:
        using value_type           = std::remove_cv_t<TElement>;
        using reference_type       = value_type&;
        using const_reference_type = value_type const&;

        using expected_reference_type       = std::expected<meta::ref_proxy<reference_type>, data_structure_error>;
        using expected_const_reference_type = std::expected<meta::ref_proxy<const_reference_type>, data_structure_error>;

        using index_type        = base_sparse_set<TIndex>::index_type;
        using sparse_index_type = base_sparse_set<TIndex>::sparse_index_type;

        static constexpr bool is_const_set = std::is_const_v<TElement>;

        /**
         * Value representing a null element in the sparse array.
         */
        static constexpr index_type null_element{ 0U };


        /**
         * @complexity O(1)
         * @param index The sparse index to check.
         * @return True if element exists at @index.
         */
        [[nodiscard]] auto has( index_type index ) const noexcept -> bool override
        {
            return index < sparse_.size( ) && sparse_[index] != null_element;
        }


        /**
          * Reserve space for at least count elements in packed storage.
          * @complexity O(1) if no reallocation needed
          */
        auto reserve( std::size_t count ) noexcept -> void
        {
            packed_.reserve( count );
            elements_.reserve( count );
        }


        /**
          * @brief Constructs an element at the given index with provided arguments.
          *
          * @complexity O(1) amortized (may trigger array resize)
          *
          * @tparam TArgs
          * @param index
          * @param args
          * @return std::expected containing the new element at the given index, or an error if the index already has an element.
          */
        template <typename... TArgs> requires std::constructible_from<value_type, TArgs...>
        auto insert(
            index_type index, TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<value_type>) -> expected_reference_type
        {
            if ( has( index ) )
            {
                return std::unexpected{ data_structure_error::index_already_exists };
            }

            // 1. ensure we can accommodate the index
            ensure_size( index );

            // 2. add new component
            sparse_[index] = encode_sparse_index( static_cast<index_type>( packed_.size( ) ) );
            packed_.emplace_back( index );
            return elements_.emplace_back( std::forward<TArgs>( args )... );
        }


        /**
          * @brief Constructs or replaces an element at the given index with provided arguments.
          * @note This function only works if TElement is not const.
          *
          * @complexity O(1) amortized (may trigger array resize)
          *
          * @tparam TArgs
          * @param index
          * @param args
          * @return The new element at the given index.
          */
        template <typename... TArgs> requires ( std::constructible_from<value_type, TArgs...> && not is_const_set )
        auto insert_or_replace(
            index_type index, TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<value_type>) -> reference_type
        {
            // 1. ensure we can accommodate the index
            ensure_size( index );

            // 2a. if we already have this entry, we replace it...
            if ( has( index ) )
            {
                index_type const decoded = decode_sparse_index( sparse_[index] );
                elements_[decoded]       = value_type{ std::forward<TArgs>( args )... };
                return elements_[decoded];
            }

            // 2b. ... else add new component
            sparse_[index] = encode_sparse_index( static_cast<index_type>( packed_.size( ) ) );
            packed_.emplace_back( index );
            return elements_.emplace_back( std::forward<TArgs>( args )... );
        }


        /**
         * Removes the element at the given index if it exists. If the element is not the set, nothing happens.
         * @complexity O(1)
         * @param index The index of the element to remove.
         */
        auto remove( index_type index ) noexcept(std::is_nothrow_destructible_v<value_type>) -> void override
        {
            // if we don't have this entry, nothing to do
            if ( not has( index ) ) { return; }

            // 1. find the position of the element to remove in the packed array
            index_type const element_pos      = decode_sparse_index( sparse_[index] );
            index_type const last_element_pos = static_cast<index_type>( packed_.size( ) ) - 1U;

            // 2. if not already removing the last element, swap with the last element to keep packed the array at low expense
            if ( bool const is_last = element_pos == last_element_pos; not is_last )
            {
                packed_[element_pos]          = packed_[last_element_pos];
                elements_[element_pos]        = std::move( elements_[last_element_pos] );
                sparse_[packed_[element_pos]] = encode_sparse_index( element_pos );
            }

            // 3. remove last element and clear sparse mapping
            sparse_[index] = null_element;
            packed_.pop_back( );
            elements_.pop_back( );
        }


        /**
         * Clears the sparse set, removing all elements.
         * @complexity O(n).
         */
        auto clear( ) noexcept(std::is_nothrow_destructible_v<value_type>) -> void override
        {
            sparse_.clear( );
            packed_.clear( );
            elements_.clear( );
        }


        /**
         * @complexity O(1)
         * @param index The index of the element to get.
         * @return A mutable reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( index_type index ) noexcept -> expected_reference_type
        {
            if ( not has( index ) )
            {
                return std::unexpected{ data_structure_error::index_not_found };
            }
            return elements_[decode_sparse_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @param index The index of the element to get.
         * @return A const reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( index_type index ) const noexcept -> expected_const_reference_type
        {
            if ( not has( index ) )
            {
                return std::unexpected{ data_structure_error::index_not_found };
            }
            return elements_[decode_sparse_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @param index The index of the element to get.
         * @return A mutable reference to the element at the given index. Asserts if the index is out of bounds.
         *
         * @note Asserts on missing index.
         */
        [[nodiscard]] auto unsafe_get( index_type index ) noexcept -> reference_type
        {
            assert( has( index ) && "sparse_set::get: index doesn't have an element!" );
            return elements_[decode_sparse_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @param index The index of the element to get.
         * @return A const reference to the element at the given index. Asserts if the index is out of bounds.
         *
         * @note Asserts on missing index.
         */
        [[nodiscard]] auto unsafe_get( index_type index ) const noexcept -> const_reference_type
        {
            assert( has(index ) && "sparse_set::get: index doesn't have an element!" );
            return elements_[decode_sparse_index( sparse_[index] )];
        }


        /**
         * @brief Returns a span over the packed indices array.
         * 
         * The packed array contains the indices of all elements that exist in the sparse set,
         * stored densely for cache-friendly iteration. The order corresponds to insertion order.
         * 
         * @return A const span over the packed indices array.
         */
        [[nodiscard]] auto packed( ) const noexcept -> std::span<index_type const> override { return packed_; }


        /**
         * @brief Returns a mutable span over the elements array.
         * 
         * Provides direct access to the packed elements' storage. Elements are stored
         * in the same order as their corresponding indices in the packed array.
         * 
         * @return A mutable span over the elements array.
         */
        [[nodiscard]] auto data( ) noexcept -> std::span<value_type> { return elements_; }


        /**
         * @brief Returns a const span over the elements array.
         * 
         * Provides direct read-only access to the packed elements' storage. Elements
         * are stored in the same order as their corresponding indices in the packed array.
         * 
         * @return A const span over the elements array.
         */
        [[nodiscard]] auto data( ) const noexcept -> std::span<value_type const> { return elements_; }


        /**
         * @return Returns the number of filled elements in the sparse set.
         */
        [[nodiscard]] auto size( ) const noexcept -> std::size_t override { return packed_.size( ); }


        /**
          * @return The current capacity of the packed storage.
          */
        [[nodiscard]] auto capacity( ) const noexcept -> std::size_t override { return packed_.capacity( ); }


        /**
          * @return True if the sparse set contains no elements.
          */
        [[nodiscard]] auto empty( ) const noexcept -> bool override { return packed_.empty( ); }

    private:
        std::vector<sparse_index_type> sparse_; // TIndex -> packed index mapping
        std::vector<index_type> packed_;        // packed array of indices
        std::vector<value_type> elements_;      // TElements in same order as packed

        /**
         * Ensures that the sparse array can accommodate the given index, resizing if necessary and filling new slots
         * with null_element.
         * @param index
         */
        auto ensure_size( index_type index ) noexcept(std::is_nothrow_constructible_v<TIndex>) -> void
        {
            if ( index >= sparse_.size( ) )
            {
                sparse_.resize( index + 1U, null_element );
            }
        }


        // +--------------------------------+
        // | TRANSCODING                    |
        // +--------------------------------+
        /**
         * @param index Packed index to encode.
         * @return The positional index for the packed to sparse conversion, encoded to avoid null_element (0U).
         */
        [[nodiscard]] static auto encode_sparse_index( index_type index ) noexcept -> sparse_index_type
        {
            return index + 1U; // +1U to avoid null_element (0U)
        }


        /**
         * @param index Sparse index to decode.
         * @return The positional index for the sparse to packed conversion.
         */
        [[nodiscard]] static auto decode_sparse_index( sparse_index_type index ) noexcept -> index_type
        {
            return index - 1U; // -1U to revert encode_index
        }
    };


    // +--------------------------------+
    // | INTERSECTION ITERATOR          |
    // +--------------------------------+
    /**
      * @brief Iterator for traversing entities that have all specified components in their sparse sets.
      *
      * This iterator implements sparse set intersection by iterating through the smallest component pool
      * and checking for entity presence in all other required pools. It provides efficient traversal
      * for ECS view operations.
      *
      * @code
      * // example: find entities with both position and velocity components
      * sparse_set<position> positions{};
      * sparse_set<velocity> velocities{};
      * 
      * // add components to entities
      * positions.insert(1, position{10.0f, 20.0f});
      * positions.insert(2, position{5.0f, 15.0f});
      * velocities.insert(1, velocity{1.0f, 0.5f});
      * velocities.insert(3, velocity{2.0f, 1.0f});
      * 
      * // create intersection iterator
      * auto begin_it = sparse_intersection_iterator<uint32_t, position, velocity>::begin(positions, velocities);
      * auto end_it = sparse_intersection_iterator<uint32_t, position, velocity>::end(positions, velocities);
      * 
      * // iterate over entities that have both components (only entity 1)
      * for (auto it = begin_it; it != end_it; ++it) {
      *     uint32_t entity = *it; // entity == 1
      *     // process entity that has both position and velocity
      * }
      * @endcode
      *
      * @note Iterator invalidation: This iterator has specific invalidation behavior due to sparse set
      *       implementation details. Operations that affect iterator validity:
      *       - insert( ) - safe during iteration (appends to end, doesn't affect current positions)
      *       - remove( ) - may cause iteration anomalies (swap-and-pop can move unvisited elements)
      *       - clear( ) - fully invalidates the iterator
      *       - reserve( ) - safe (only pre-allocates, doesn't move existing elements)
      *       - Modifying element values in-place is always safe
      *
      * @tparam TIndex The entity index type (typically uint32_t).
      * @tparam TElements The element types to intersect.
      *
      * @complexity Iterator operations are O(k) where k is the number of component types.
      */
    template <std::integral TIndex, typename... TElements>
    class sparse_intersection_iterator final
    {
    public:
        using set_array_type = std::array<base_sparse_set<TIndex> const*, sizeof...( TElements )>;


        // +--------------------------------+
        // | FACTORIES                      |
        // +--------------------------------+
        /**
         * Find the smallest set and create a begin-iterator for that.
         * @param sets
         * @return
         */
        static auto begin( sparse_set<TElements, TIndex>&... sets ) noexcept -> sparse_intersection_iterator
        {
            auto& smallest_set = *meta::find_smallest<base_sparse_set<TIndex>>( sets... );
            return sparse_intersection_iterator{ set_array_type{ &sets... }, smallest_set, 0U };
        }


        /**
         * Find the smallest set and create an end-iterator for that.
         * @param sets
         * @return
         */
        static auto end( sparse_set<TElements, TIndex>&... sets ) noexcept -> sparse_intersection_iterator
        {
            auto& smallest_set = *meta::find_smallest<base_sparse_set<TIndex>>( sets... );
            return sparse_intersection_iterator{
                set_array_type{ &sets... }, smallest_set, static_cast<TIndex>( smallest_set.size( ) )
            };
        }


        /**
         * Create an iterator that intersects multiple sparse sets.
         * @param sets
         * @param pivot_set The set used for iteration. It should be the smallest one.
         * @param pos
         */
        explicit sparse_intersection_iterator(
            set_array_type const& sets, base_sparse_set<TIndex> const& pivot_set, TIndex const pos ) noexcept
            : packed_pos_{ pos }
            , sets_{ sets }
            , pivot_set_ref_{ pivot_set }
        {
            // make sure packed pos is in range
            packed_pos_ = std::clamp( pos, TIndex{ 0U }, static_cast<TIndex>( pivot_set_ref_.size( ) ) );

            // advance to the first valid intersected entity
            advance_to_valid( );
        }


        auto operator*( ) const noexcept -> TIndex
        {
            return pivot_set_ref_.packed( )[packed_pos_];
        }


        auto operator++( ) noexcept -> sparse_intersection_iterator&
        {
            ++packed_pos_;
            advance_to_valid( );
            return *this;
        }


        auto operator++( int ) noexcept -> sparse_intersection_iterator
        {
            auto temp = *this;
            ++( *this );
            return temp;
        }


        auto operator==( sparse_intersection_iterator const& other ) const noexcept -> bool
        {
            return packed_pos_ == other.packed_pos_;
        }


        auto operator!=( sparse_intersection_iterator const& other ) const noexcept -> bool
        {
            return packed_pos_ != other.packed_pos_;
        }

    private:
        TIndex packed_pos_{ 0U };
        set_array_type const& sets_{};
        base_sparse_set<TIndex> const& pivot_set_ref_;


        auto advance_to_valid( ) noexcept -> void
        {
            while ( packed_pos_ < pivot_set_ref_.size( ) )
            {
                if ( TIndex const current_entity = pivot_set_ref_.packed( )[packed_pos_];
                    std::ranges::all_of( sets_, [current_entity]( auto const* set ) { return set->has( current_entity ); } ) )
                {
                    break;
                }
                ++packed_pos_;
            }
        }
    };
}


#endif //!RST_SPARSE_SET_H
