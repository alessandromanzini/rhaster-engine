#ifndef RST_EVENT_QUEUE_H
#define RST_EVENT_QUEUE_H

#include <rst/pch.h>


namespace rst
{
    template <typename TEvent>
    class EventQueue final
    {
    public:
        EventQueue( )  = default;
        ~EventQueue( ) = default;

        EventQueue( EventQueue const& );
        EventQueue( EventQueue&& ) noexcept;
        auto operator=( EventQueue const& ) -> EventQueue&     = delete;
        auto operator=( EventQueue&& ) noexcept -> EventQueue& = delete;

        auto push( TEvent&& event ) -> void;
        auto push( TEvent const& event ) -> void;

        auto front( ) -> TEvent&;
        auto front( ) const -> TEvent const&;

        auto pop( ) -> void;

        auto empty( ) const -> bool;

        /**
         * Retrieves the first element of the queue and pops it.
         * @param event The popped event
         * @return false if the queue was empty, true otherwise
         */
        auto pop( TEvent& event ) -> bool;

        /**
         * Tests if the queue contains the event.
         * @param event
         * @return
         */
        auto is_queued( TEvent const& event ) const -> bool;
        auto is_queued( std::function<bool( TEvent const& )> const& predicate ) const -> bool;

        auto clear( ) -> void;

    private:
        std::deque<TEvent> queue_;
    };


    template <typename event_t>
    EventQueue<event_t>::EventQueue( EventQueue const& other ) : queue_{ other.queue_ } { }


    template <typename event_t>
    EventQueue<event_t>::EventQueue( EventQueue&& other ) noexcept : queue_{ std::move( other.queue_ ) } { }


    template <typename event_t>
    auto EventQueue<event_t>::push( event_t&& event ) -> void
    {
        queue_.emplace_back( std::move( event ) );
    }


    template <typename event_t>
    auto EventQueue<event_t>::push( event_t const& event ) -> void
    {
        queue_.push_back( event );
    }


    template <typename event_t>
    auto EventQueue<event_t>::front( ) -> event_t&
    {
        return queue_.front( );
    }


    template <typename event_t>
    auto EventQueue<event_t>::front( ) const -> event_t const&
    {
        return queue_.front( );
    }


    template <typename event_t>
    auto EventQueue<event_t>::pop( ) -> void
    {
        queue_.pop_front( );
    }


    template <typename event_t>
    auto EventQueue<event_t>::empty( ) const -> bool
    {
        return queue_.empty( );
    }


    template <typename event_t>
    auto EventQueue<event_t>::pop( event_t& event ) -> bool
    {
        if ( empty( ) )
        {
            return false;
        }
        event = front( );
        pop( );
        return true;
    }


    template <typename event_t>
    auto EventQueue<event_t>::is_queued( event_t const& event ) const -> bool
    {
        if constexpr ( std::equality_comparable<event_t> )
        {
            return queue_.contains( event );
        }
        else
        {
            assert( false && "Event type is not equality comparable!" );
        }
    }


    template <typename event_t>
    auto EventQueue<event_t>::is_queued( std::function<bool( event_t const& )> const& predicate ) const -> bool
    {
        return queue_.contains( predicate );
    }


    template <typename event_t>
    auto EventQueue<event_t>::clear( ) -> void
    {
        queue_.clear( );
    }
}


#endif //! RST_EVENT_QUEUE_H
