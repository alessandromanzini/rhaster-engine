#ifndef RST_SPARSE_SET_H
#define RST_SPARSE_SET_H

#include <rst/pch.h>


// TODO: add documentation and move to appropriate folder
namespace rst
{
    template <typename TElement>
    class SparseSet
    {
    public:
        auto insert( TElement&& element ) -> void
        {
            if ( elements_to_index_.contains( element ) ) { return; }

            size_t index = elements_.size( );
            elements_.push_back( element );
            elements_to_index_[element] = index;
        }


        auto remove( TElement const& element ) -> void
        {
            auto it = elements_to_index_.find( element );
            if ( it == elements_to_index_.end( ) ) { return; }

            size_t idx    = it->second;
            TElement last = elements_.back( );

            elements_[idx]           = last;
            elements_to_index_[last] = idx;

            elements_.pop_back( );
            elements_to_index_.erase( element );
        }


        auto clear( ) -> void
        {
            elements_.clear( );
            elements_to_index_.clear( );
        }


        auto contains( TElement const& element ) const -> bool
        {
            return elements_to_index_.contains( element );
        }


        auto data( ) const -> std::vector<TElement> const&
        {
            return elements_;
        }

    private:
        std::vector<TElement> elements_;
        std::map<TElement, size_t> elements_to_index_;
    };
}


#endif //!RST_SPARSE_SET_H
