#include <rst/__event/subject.h>

#include <rst/__event/observer.h>


namespace rst
{
    auto Subject::add_observer( Observer& observer ) -> void
    {
        // add the observer to the front of the list
        observer.next_ptr_ = head_ptr_;
        head_ptr_          = &observer;
    }


    auto Subject::remove_observer( Observer const& observer ) -> void
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


    auto Subject::broadcast( Uid const event, event::BroadcastValue const& value ) const -> void
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
