#ifndef RST_EVENT_QUEUE_H
#define RST_EVENT_QUEUE_H

#include <rst/pch.h>


namespace rst
{
    template <typename TEvent>
    class event_queue final
    {
    public:
        event_queue( )  = default;
        ~event_queue( ) = default;

        event_queue( event_queue const& );
        event_queue( event_queue&& ) noexcept;
        auto operator=( event_queue const& ) -> event_queue&     = delete;
        auto operator=( event_queue&& ) noexcept -> event_queue& = delete;

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


    template <typename TEvent>
    event_queue<TEvent>::event_queue( event_queue const& other ) : queue_{ other.queue_ } { }


    template <typename TEvent>
    event_queue<TEvent>::event_queue( event_queue&& other ) noexcept : queue_{ std::move( other.queue_ ) } { }


    template <typename TEvent>
    auto event_queue<TEvent>::push( TEvent&& event ) -> void
    {
        queue_.emplace_back( std::move( event ) );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::push( TEvent const& event ) -> void
    {
        queue_.push_back( event );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::front( ) -> TEvent&
    {
        return queue_.front( );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::front( ) const -> TEvent const&
    {
        return queue_.front( );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::pop( ) -> void
    {
        queue_.pop_front( );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::empty( ) const -> bool
    {
        return queue_.empty( );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::pop( TEvent& event ) -> bool
    {
        if ( empty( ) )
        {
            return false;
        }
        event = front( );
        pop( );
        return true;
    }


    template <typename TEvent>
    auto event_queue<TEvent>::is_queued( TEvent const& event ) const -> bool
    {
        if constexpr ( std::equality_comparable<TEvent> )
        {
            return queue_.contains( event );
        }
        else
        {
            assert( false && "Event type is not equality comparable!" );
        }
    }


    template <typename TEvent>
    auto event_queue<TEvent>::is_queued( std::function<bool( TEvent const& )> const& predicate ) const -> bool
    {
        return queue_.contains( predicate );
    }


    template <typename TEvent>
    auto event_queue<TEvent>::clear( ) -> void
    {
        queue_.clear( );
    }
}


#endif //! RST_EVENT_QUEUE_H
