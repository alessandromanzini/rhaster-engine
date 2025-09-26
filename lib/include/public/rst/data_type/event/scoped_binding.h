#ifndef RST_EVENT_SCOPED_BINDING_H
#define RST_EVENT_SCOPED_BINDING_H

#include <rst/pch.h>

#include <rst/data_type/event/token.h>


namespace rst
{
    template <typename... TParams> class multicast_delegate;
}

namespace rst::event
{
    template <typename... TParams>
    class scoped_binding final
    {
    public:
        using event_type = multicast_delegate<TParams...>;


        scoped_binding( delegate_token_type const token, event_type& delegate ) noexcept
            : token_{ token }
            , delegate_ref_{ delegate } { }


        ~scoped_binding( ) noexcept
        {
            disconnect( );
        }


        scoped_binding( scoped_binding&& other ) noexcept
            : token_{ std::exchange( other.token_, invalid_token ) }
            , delegate_ref_{ other.delegate_ref_ } { }


        scoped_binding( scoped_binding const& )                              = delete;
        auto operator=( scoped_binding const& ) -> scoped_binding&           = delete;
        auto operator=( scoped_binding&& other ) noexcept -> scoped_binding& = delete;


        auto disconnect( ) -> void
        {
            if ( token_ != invalid_token )
            {
                delegate_ref_.unbind_token( token_ );
                token_ = invalid_token;
            }
        }


        [[nodiscard]] auto is_connected( ) const -> bool { return token_ != invalid_token; }

    private:
        delegate_token_type token_{ invalid_token };
        event_type& delegate_ref_;
    };
}


#endif //!RST_EVENT_SCOPED_BINDING_H
