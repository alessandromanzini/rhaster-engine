#ifndef DELETER_H
#define DELETER_H

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <map>
#include <unordered_set>
#include <vector>
#include <memory> // need this header for MSVC


namespace engine
{
    template<typename deletable_t>
    class Deleter final
    {
    public:
        void mark_element_for_deletion( deletable_t* element )
        {
            elements_to_destroy_.insert( element );
        }


        void cleanup( std::vector<std::unique_ptr<deletable_t>>& deletables )
        {
            std::erase_if( deletables,
                           [this]( auto& element ) { return elements_to_destroy_.count( element.get( ) ); } );
            elements_to_destroy_.clear( );
        }


        void cleanup( std::multimap<uint64_t, std::unique_ptr<deletable_t>>& deletables )
        {
            std::erase_if( deletables,
                           [this]( const auto& pair ) { return elements_to_destroy_.count( pair.second.get( ) ); } );
            elements_to_destroy_.clear( );
        }


        [[nodiscard]] bool is_cleanup_needed( ) const
        {
            return !elements_to_destroy_.empty( );
        }

    private:
        // We choose a set because we don't want duplicates and we retain O(1) complexity for
        // insertion, deletion, and search.
        std::unordered_set<deletable_t*> elements_to_destroy_{};

    };
}

#endif //!DELETER_H
