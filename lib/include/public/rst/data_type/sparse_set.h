#ifndef RST_SPARSE_SET_H
#define RST_SPARSE_SET_H

#include <rst/pch.h>

#include <rst/meta/algorithm.h>


// TODO: add documentation and move to appropriate folder
namespace rst
{
    // +--------------------------------+
    // | BASE SPARSE SET CLASS          |
    // +--------------------------------+
    template <std::integral TIndex>
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
     * @tparam TElement The type of elements stored.
     * @tparam TIndex The index type (must be integral).
     */
    template <std::move_constructible TElement, std::integral TIndex = uint32_t>
    class sparse_set final : public base_sparse_set<TIndex>
    {
    public:
        using element_type      = std::remove_const_t<TElement>;
        using index_type        = base_sparse_set<TIndex>::index_type;
        using sparse_index_type = base_sparse_set<TIndex>::sparse_index_type;


        /**
         * Value representing a null element in the sparse array.
         */
        static constexpr index_type null_element{ 0U };


        /**
         * @complexity O(1)
         * @param index The sparse index to check
         * @return True if element exists at @index
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
          * Inserts a new element at the given index, constructing it with the provided arguments.
          * @complexity O(1) amortized (may trigger array resize)
          * @tparam TArgs
          * @param index
          * @param args
          * @return
          */
        template <typename... TArgs> requires std::constructible_from<element_type, TArgs...>
        auto insert( index_type index, TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<element_type>) -> element_type&
        {
            // 1. ensure we can accommodate the index
            ensure_size( index );

            // 2a. if we already have this entry, we replace it...
            if ( has( index ) )
            {
                index_type const decoded = decode_sparse_index( sparse_[index] );
                elements_[decoded]       = element_type{ std::forward<TArgs>( args )... };
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
         * @param index
         */
        auto remove( index_type index ) noexcept(std::is_nothrow_destructible_v<element_type>) -> void override
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
         * @complexity O(n)
         */
        auto clear( ) noexcept(std::is_nothrow_destructible_v<element_type>) -> void override
        {
            sparse_.clear( );
            packed_.clear( );
            elements_.clear( );
        }


        /**
         * @complexity O(1)
         * @param index
         * @return A mutable reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( index_type index ) noexcept -> element_type&
        {
            assert( has( index ) && "sparse_set::get: index doesn't have an element!" );
            return elements_[decode_sparse_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @param index
         * @return A const reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( index_type index ) const noexcept -> element_type const&
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
        [[nodiscard]] auto data( ) noexcept -> std::span<element_type> { return elements_; }


        /**
         * @brief Returns a const span over the elements array.
         * 
         * Provides direct read-only access to the packed elements' storage. Elements
         * are stored in the same order as their corresponding indices in the packed array.
         * 
         * @return A const span over the elements array.
         */
        [[nodiscard]] auto data( ) const noexcept -> std::span<element_type const> { return elements_; }


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
        std::vector<element_type> elements_;    // TElements in same order as packed

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
         * @tparam TIndex The entity index type (typically uint32_t)
         * @tparam TComponents The component types to intersect
         *
         * @complexity Iterator operations are O(k) where k is the number of component types
         */
    template <std::integral TIndex, typename... TComponents>
    class sparse_intersection_iterator final
    {
    public:
        using sparse_sets_type = std::tuple<sparse_set<TComponents, TIndex>&...>;

        // +--------------------------------+
        // | FACTORIES                      |
        // +--------------------------------+
        static auto begin( sparse_set<TComponents, TIndex>&... sets ) noexcept -> sparse_intersection_iterator
        {
            auto& smallest_set = *meta::find_smallest<base_sparse_set<TIndex>>( sets... );
            return sparse_intersection_iterator{ sparse_sets_type{ sets... }, smallest_set, 0U };
        }


        static auto end( sparse_set<TComponents, TIndex>&... sets ) noexcept -> sparse_intersection_iterator
        {
            auto& smallest_set = *meta::find_smallest<base_sparse_set<TIndex>>( sets... );
            return sparse_intersection_iterator{
                sparse_sets_type{ sets... }, smallest_set, static_cast<TIndex>( smallest_set.size( ) )
            };
        }

        auto operator*( ) const noexcept -> TIndex
        {
            return smallest_set_ref_.packed( )[packed_pos_];
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
        sparse_sets_type sets_{};
        base_sparse_set<TIndex> const& smallest_set_ref_;


        explicit sparse_intersection_iterator(
            sparse_sets_type const sets, base_sparse_set<TIndex> const& smallest, TIndex const pos ) noexcept
            : packed_pos_{ pos }
            , sets_{ sets }
            , smallest_set_ref_{ smallest }
        {
            // make sure packed pos is in range
            packed_pos_ = std::clamp( pos, TIndex{ 0U }, static_cast<TIndex>( smallest_set_ref_.size( ) ) );

            // advance to the first valid intersected entity
            advance_to_valid( );
        }


        auto advance_to_valid( ) noexcept -> void
        {
            while ( packed_pos_ < smallest_set_ref_.size( ) )
            {
                TIndex const current_entity = smallest_set_ref_.packed( )[packed_pos_];

                bool const is_valid = std::apply(
                    [current_entity]( sparse_set<TComponents, TIndex>&... sets )
                    {
                        return ( sets.has( current_entity ) && ... );
                    }, sets_ );

                if ( is_valid )
                {
                    break;
                }
                ++packed_pos_;
            }
        }
    };
}


#endif //!RST_SPARSE_SET_H
