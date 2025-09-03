#ifndef RST_BASE_CONTROLLER_H
#define RST_BASE_CONTROLLER_H

#include <rst/pch.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    class base_controller
    {
    public:
        virtual ~base_controller( ) = default;

        base_controller( base_controller const& )                        = delete;
        base_controller( base_controller&& ) noexcept                    = delete;
        auto operator=( base_controller const& ) -> base_controller&     = delete;
        auto operator=( base_controller&& ) noexcept -> base_controller& = delete;

        /**
         * This method is called when a controller possesses a GameObject.
         * @param pawn The pointer to the possessed GameObject
         */
        virtual auto possess( gameobject* pawn ) -> void;

        /**
         * This method is called when a controller unpossesses a GameObject.
         */
        virtual auto unpossess( ) -> void;

        [[nodiscard]] auto pawn( ) const -> gameobject const*;
        [[nodiscard]] auto pawn( ) -> gameobject*;

        [[nodiscard]] auto has_pawn( ) const -> bool;

        [[nodiscard]] auto id( ) const -> int32_t;

        [[nodiscard]] auto operator==( base_controller const& other ) const -> bool;
        [[nodiscard]] auto operator==( base_controller const* other ) const -> bool;

    protected:
        int32_t const id_;

        base_controller( );

    private:
        inline static int32_t s_id_counter_{ 0 };
        gameobject* pawn_ptr_{ nullptr };
    };


    inline auto base_controller::possess( gameobject* pawn ) -> void
    {
        // TODO: add error logging system
        assert( pawn && "BaseController::possess: Pawn cannot be nullptr!" );

        if ( pawn_ptr_ )
        {
            unpossess( );
        }
        pawn_ptr_ = pawn;
        pawn_ptr_->on_deletion.bind( this, &base_controller::unpossess );
    }


    inline auto base_controller::unpossess( ) -> void
    {
        pawn_ptr_->on_deletion.unbind( this );
        pawn_ptr_ = nullptr;
    }


    inline auto base_controller::pawn( ) const -> gameobject const*
    {
        return pawn_ptr_;
    }


    inline auto base_controller::pawn( ) -> gameobject*
    {
        return pawn_ptr_;
    }


    inline auto base_controller::has_pawn( ) const -> bool
    {
        return pawn_ptr_ != nullptr;
    }


    inline auto base_controller::id( ) const -> int32_t
    {
        return id_;
    }


    inline auto base_controller::operator==( base_controller const& other ) const -> bool
    {
        return id_ == other.id_;
    }


    inline auto base_controller::operator==( base_controller const* other ) const -> bool
    {
        return id_ == other->id_;
    }


    inline base_controller::base_controller( ) : id_{ s_id_counter_++ } { }
}


#endif //!RST_BASE_CONTROLLER_H
