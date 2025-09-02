#ifndef RST_DELETER_H
#define RST_DELETER_H

#include <rst/pch.h>


namespace rst
{
    template <typename deletable_t>
    class Deleter final
    {
    public:
        auto mark_element_for_deletion( deletable_t* element ) -> void
        {
            elements_to_destroy_.insert( element );
        }


        auto cleanup( std::vector<std::unique_ptr<deletable_t>>& deletables ) -> void
        {
            std::erase_if( deletables, [this]( auto& element ) { return elements_to_destroy_.count( element.get( ) ); } );
            elements_to_destroy_.clear( );
        }


        auto cleanup( std::multimap<uint64_t, std::unique_ptr<deletable_t>>& deletables ) -> void
        {
            std::erase_if( deletables, [this]( auto const& pair ) { return elements_to_destroy_.count( pair.second.get( ) ); } );
            elements_to_destroy_.clear( );
        }


        [[nodiscard]] auto is_cleanup_needed( ) const -> bool
        {
            return !elements_to_destroy_.empty( );
        }

    private:
        // we choose a set because we don't want duplicates and we retain O(1) complexity for
        // insertion, deletion, and search.
        std::unordered_set<deletable_t*> elements_to_destroy_{};
    };
}


#endif //!RST_DELETER_H
