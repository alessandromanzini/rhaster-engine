#ifndef RST_SPARSE_SET_H
#define RST_SPARSE_SET_H

#include <rst/pch.h>


// TODO: add documentation and move to appropriate folder
namespace rst
{
    // +--------------------------------+
    // | BASE SPARSE SET CLASS          |
    // +--------------------------------+
    template <typename TIndex>
    class base_sparse_set
    {
    public:
        base_sparse_set( )                   = default;
        virtual ~base_sparse_set( ) noexcept = default;

        base_sparse_set( base_sparse_set const& )                        = delete;
        base_sparse_set( base_sparse_set&& ) noexcept                    = delete;
        auto operator=( base_sparse_set const& ) -> base_sparse_set&     = delete;
        auto operator=( base_sparse_set&& ) noexcept -> base_sparse_set& = delete;

        virtual auto has( TIndex index ) const -> bool = 0;
        virtual auto remove( TIndex index ) -> void = 0;
        virtual auto clear( ) -> void = 0;
    };


    // +--------------------------------+
    // | SPARSE SET TEMPLATED IMPL      |
    // +--------------------------------+
    /**
     * @brief A sparse set data structure providing O(1) operations for component storage.
     *
     * Sparse sets maintain two arrays: a sparse array for O(1) lookups and a packed
     * array for cache-friendly iteration. Ideal for ECS component storage where entity
     * IDs may be sparse but component iteration should be dense.
     *
     * @tparam TElement The type of elements stored
     * @tparam TIndex The index type (must be integral)
     */
    template <typename TElement, typename TIndex = uint32_t> requires
        std::is_integral_v<TIndex> && std::is_move_assignable_v<TElement>
    class sparse_set final : public base_sparse_set<TIndex>
    {
    public:
        /**
         * Value representing a null element in the sparse array.
         */
        static constexpr TIndex null_element = 0U;

#pragma region interface_overloads
        /**
         * @complexity O(1)
         * @param index The sparse index to check
         * @return True if element exists at @index
         */
        [[nodiscard]] auto has( TIndex index ) const -> bool override
        {
            return index < sparse_.size( ) && sparse_[index] != null_element;
        }


        /**
         * Removes the element at the given index if it exists. If the element is not the set, nothing happens.
         * @complexity O(1)
         * @param index
         */
        auto remove( TIndex index ) -> void override
        {
            // if we don't have this entry, nothing to do
            if ( not has( index ) ) { return; }

            // 1. find the position of the element to remove in the packed array
            TIndex const element_pos    = decode_index( sparse_[index] );
            auto const last_element_pos = static_cast<TIndex>( dense_.size( ) ) - 1U;

            // 2. if not already removing the last element, swap with the last element to keep dense array
            if ( bool const is_last = element_pos == last_element_pos; not is_last )
            {
                dense_[element_pos]          = dense_[last_element_pos];
                elements_[element_pos]       = std::move( elements_[last_element_pos] );
                sparse_[dense_[element_pos]] = encode_index( element_pos );
            }

            // 3. remove last element and clear sparse mapping
            sparse_[index] = null_element;
            dense_.pop_back( );
            elements_.pop_back( );
        }


        /**
         * Clears the sparse set, removing all elements.
         * @complexity O(1)
         */
        auto clear( ) -> void override
        {
            sparse_.clear( );
            dense_.clear( );
            elements_.clear( );
        }
#pragma endregion

#pragma region mutators
        /**
          * Inserts a new element at the given index, constructing it with the provided arguments.
          * @complexity O(1) amortized (may trigger array resize)
          * @tparam TArgs
          * @param index
          * @param args
          * @return
          */
        template <typename... TArgs> requires std::constructible_from<TElement, TArgs...>
        auto insert( TIndex index, TArgs&&... args ) -> TElement&
        {
            // 1. ensure we can accommodate the index
            ensure_size( index );

            // 2a. if we already have this entry, we replace it...
            if ( has( index ) )
            {
                TIndex const pos = decode_index( sparse_[index] );
                elements_[pos]   = TElement{ std::forward<TArgs>( args )... };
                return elements_[pos];
            }

            // 2b. ... else add new component
            sparse_[index] = encode_index( static_cast<TIndex>( dense_.size( ) ) );
            dense_.emplace_back( index );
            return elements_.emplace_back( std::forward<TArgs>( args )... );
        }


        /**
          * Reserve space for at least count elements in packed storage.
          * @complexity O(1) if no reallocation needed
          */
        auto reserve( std::size_t count ) -> void
        {
            dense_.reserve( count );
            elements_.reserve( count );
        }
#pragma endregion

#pragma region accessors
        /**
         * @complexity O(1)
         * @param index
         * @return A mutable reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( TIndex index ) -> TElement&
        {
            assert( has( index ) && "sparse_set::get: index doesn't have an element!" );
            return elements_[decode_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @param index
         * @return A const reference to the element at the given index. Asserts if the index is out of bounds.
         */
        [[nodiscard]] auto get( TIndex index ) const -> TElement const&
        {
            assert( has(index ) && "sparse_set::get: index doesn't have an element!" );
            return elements_[decode_index( sparse_[index] )];
        }


        /**
         * @complexity O(1)
         * @return A view of the packed indices in the sparse set.
         */
        [[nodiscard]] auto dense( ) const -> std::span<TIndex const> { return dense_; }


        /**
         * @return Returns the number of filled elements in the sparse set.
         */
        [[nodiscard]] auto size( ) const -> std::size_t { return dense_.size( ); }


        /**
          * @return The current capacity of the packed storage.
          */
        [[nodiscard]] auto capacity( ) const -> std::size_t { return elements_.capacity( ); }


        /**
          * @return True if the sparse set contains no elements.
          */
        [[nodiscard]] auto empty( ) const -> bool { return dense_.empty( ); }
#pragma endregion

#pragma region iterators
        /**
         * @return A mutable iterator of the first set element.
         */
        [[nodiscard]] auto begin( ) { return elements_.begin( ); }

        /**
         * @return A mutable iterator of the last set element.
         */
        [[nodiscard]] auto end( ) { return elements_.end( ); }

        /**
         * @return A const iterator of the first set element.
         */
        [[nodiscard]] auto begin( ) const { return elements_.begin( ); }
        [[nodiscard]] auto cbegin( ) const { return begin( ); }

        /**
         * @return A const iterator of the last set element.
         */
        [[nodiscard]] auto end( ) const { return elements_.end( ); }
        [[nodiscard]] auto cend( ) const { return end( ); }
#pragma endregion

    private:
        std::vector<TIndex> sparse_;     // TIndex -> dense index mapping
        std::vector<TIndex> dense_;      // dense array of indices
        std::vector<TElement> elements_; // TElements in same order as dense

        /**
         * Ensures that the sparse array can accommodate the given index, resizing if necessary and filling new slots
         * with null_element.
         * @param index
         */
        auto ensure_size( TIndex index ) -> void
        {
            if ( index >= sparse_.size( ) )
            {
                sparse_.resize( index + 1U, null_element );
            }
        }


        /**
         * @param index Packed index to encode.
         * @return The positional index for the packed to sparse conversion, encoded to avoid null_element (0U).
         */
        [[nodiscard]] static auto encode_index( TIndex index ) -> TIndex
        {
            return index + 1U; // +1U to avoid null_element (0U)
        }


        /**
         * @param index Sparse index to decode.
         * @return The positional index for the sparse to packed conversion.
         */
        [[nodiscard]] static auto decode_index( TIndex index ) -> TIndex
        {
            return index - 1U; // -1U to revert encode_index
        }
    };
}


#endif //!RST_SPARSE_SET_H
