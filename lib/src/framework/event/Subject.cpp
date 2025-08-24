#include <framework/event/Subject.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/event/Observer.h>


namespace engine
{
    void Subject::add_observer( Observer& observer )
    {
        // Add the observer to the front of the list
        observer.next_ptr_ = head_ptr_;
        head_ptr_          = &observer;
    }


    void Subject::remove_observer( const Observer& observer )
    {
        if ( head_ptr_ == &observer )
        {
            head_ptr_ = observer.next_ptr_;
        }
        else
        {
            Observer* current = head_ptr_;
            while ( current->next_ptr_ != &observer )
            {
                current = current->next_ptr_;
            }
            current->next_ptr_ = observer.next_ptr_;
        }
    }


    void Subject::broadcast( const UID event, const event::broadcast_value_variant_t& value ) const
    {
        if ( head_ptr_ == nullptr ){ return; }

        Observer* current = head_ptr_;
        while ( current != nullptr )
        {
            current->notify( event, value );
            current = current->next_ptr_;
        }
    }

}
