#include <rst/__event/subject.h>

#include <rst/__event/observer.h>


namespace rst
{
    auto subject::add_observer( observer& observer ) -> void
    {
        // add the observer to the front of the list
        observer.next_ptr_ = head_ptr_;
        head_ptr_          = &observer;
    }


    auto subject::remove_observer( observer const& observer ) -> void
    {
        if ( head_ptr_ == &observer )
        {
            head_ptr_ = observer.next_ptr_;
        }
        else
        {
            rst::observer* current = head_ptr_;
            while ( current->next_ptr_ != &observer )
            {
                current = current->next_ptr_;
            }
            current->next_ptr_ = observer.next_ptr_;
        }
    }


    auto subject::broadcast( earmark const event, event::broadcast_value_type const& value ) const -> void
    {
        if ( head_ptr_ == nullptr ){ return; }

        observer* current = head_ptr_;
        while ( current != nullptr )
        {
            current->notify( event, value );
            current = current->next_ptr_;
        }
    }

}
