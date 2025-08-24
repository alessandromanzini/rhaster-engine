#ifndef SPARSESET_H
#define SPARSESET_H

#include <vector>
#include <map>


namespace engine
{
    template <typename element_t>
    class SparseSet
    {
    public:
        void insert( element_t&& element )
        {
            if ( elements_to_index_.contains( element ) ) { return; }

            size_t index = elements_.size( );
            elements_.push_back( element );
            elements_to_index_[element] = index;
        }


        void remove( const element_t& element )
        {
            auto it = elements_to_index_.find( element );
            if ( it == elements_to_index_.end( ) ) { return; }

            size_t idx     = it->second;
            element_t last = elements_.back( );

            elements_[idx]           = last;
            elements_to_index_[last] = idx;

            elements_.pop_back( );
            elements_to_index_.erase( element );
        }


        void clear( )
        {
            elements_.clear( );
            elements_to_index_.clear( );
        }


        bool contains( const element_t& element ) const
        {
            return elements_to_index_.contains( element );
        }


        const std::vector<element_t>& data( ) const
        {
            return elements_;
        }

    private:
        std::vector<element_t> elements_;
        std::map<element_t, size_t> elements_to_index_;

    };

}


#endif //!SPARSESET_H
