#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <cassert>
#include <deque>


namespace engine
{
    template <typename event_t>
    class EventQueue final
    {
    public:
        EventQueue( )  = default;
        ~EventQueue( ) = default;

        EventQueue( const EventQueue& );
        EventQueue( EventQueue&& ) noexcept;
        EventQueue& operator=( const EventQueue& )     = delete;
        EventQueue& operator=( EventQueue&& ) noexcept = delete;

        void push( event_t&& event );
        void push( const event_t& event );

        event_t& front( );
        const event_t& front( ) const;

        void pop( );

        bool empty( ) const;

        /**
         * Retrieves the first element of the queue and pops it.
         * @param event The popped event
         * @return false if the queue was empty, true otherwise
         */
        bool pop( event_t& event );

        /**
         * Tests if the queue contains the event.
         * @param event
         * @return
         */
        bool is_queued( const event_t& event ) const;
        bool is_queued( const std::function<bool(const event_t&)>& predicate ) const;

        void clear( );

    private:
        std::deque<event_t> queue_;

    };


    template <typename event_t>
    EventQueue<event_t>::EventQueue( const EventQueue& other )
        : queue_{ other.queue_ }
    {
    }


    template <typename event_t>
    EventQueue<event_t>::EventQueue( EventQueue&& other ) noexcept
        : queue_{ std::move( other.queue_ ) }
    {
    }


    template <typename event_t>
    void EventQueue<event_t>::push( event_t&& event )
    {
        queue_.emplace_back( std::move( event ) );
    }


    template <typename event_t>
    void EventQueue<event_t>::push( const event_t& event )
    {
        queue_.push_back( event );
    }


    template <typename event_t>
    event_t& EventQueue<event_t>::front( )
    {
        return queue_.front( );
    }


    template <typename event_t>
    const event_t& EventQueue<event_t>::front( ) const
    {
        return queue_.front( );
    }


    template <typename event_t>
    void EventQueue<event_t>::pop( )
    {
        queue_.pop_front( );
    }


    template <typename event_t>
    bool EventQueue<event_t>::empty( ) const
    {
        return queue_.empty( );
    }


    template <typename event_t>
    bool EventQueue<event_t>::pop( event_t& event )
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
    bool EventQueue<event_t>::is_queued( const event_t& event ) const
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
    bool EventQueue<event_t>::is_queued( const std::function<bool(const event_t&)>& predicate ) const
    {
        return queue_.contains( predicate );
    }


    template <typename event_t>
    void EventQueue<event_t>::clear( )
    {
        queue_.clear( );
    }

}


#endif //!EVENTQUEUE_H
