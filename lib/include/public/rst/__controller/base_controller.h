#ifndef RST_BASE_CONTROLLER_H
#define RST_BASE_CONTROLLER_H

#include <rst/pch.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    class BaseController
    {
    public:
        virtual ~BaseController( ) = default;

        BaseController( BaseController const& )                        = delete;
        BaseController( BaseController&& ) noexcept                    = delete;
        auto operator=( BaseController const& ) -> BaseController&     = delete;
        auto operator=( BaseController&& ) noexcept -> BaseController& = delete;

        /**
         * This method is called when a controller possesses a GameObject.
         * @param pawn The pointer to the possessed GameObject
         */
        virtual auto possess( GameObject* pawn ) -> void;

        /**
         * This method is called when a controller unpossesses a GameObject.
         */
        virtual auto unpossess( ) -> void;

        [[nodiscard]] auto get_pawn( ) const -> GameObject const*;
        [[nodiscard]] auto get_pawn( ) -> GameObject*;

        [[nodiscard]] auto has_pawn( ) const -> bool;

        [[nodiscard]] auto get_id( ) const -> int32_t;

        [[nodiscard]] auto operator==( BaseController const& other ) const -> bool;
        [[nodiscard]] auto operator==( BaseController const* other ) const -> bool;

    protected:
        int32_t const id_;

        BaseController( );

    private:
        inline static int32_t s_id_counter_{ 0 };
        GameObject* pawn_ptr_{ nullptr };
    };


    inline auto BaseController::possess( GameObject* pawn ) -> void
    {
        // TODO: add error logging system
        assert( pawn && "BaseController::possess: Pawn cannot be nullptr!" );

        if ( pawn_ptr_ )
        {
            unpossess( );
        }
        pawn_ptr_ = pawn;
        pawn_ptr_->on_deletion.bind( this, &BaseController::unpossess );
    }


    inline auto BaseController::unpossess( ) -> void
    {
        pawn_ptr_->on_deletion.unbind( this );
        pawn_ptr_ = nullptr;
    }


    inline auto BaseController::get_pawn( ) const -> GameObject const*
    {
        return pawn_ptr_;
    }


    inline auto BaseController::get_pawn( ) -> GameObject*
    {
        return pawn_ptr_;
    }


    inline auto BaseController::has_pawn( ) const -> bool
    {
        return pawn_ptr_ != nullptr;
    }


    inline auto BaseController::get_id( ) const -> int32_t
    {
        return id_;
    }


    inline auto BaseController::operator==( BaseController const& other ) const -> bool
    {
        return id_ == other.id_;
    }


    inline auto BaseController::operator==( BaseController const* other ) const -> bool
    {
        return id_ == other->id_;
    }


    inline BaseController::BaseController( ) : id_{ s_id_counter_++ } { }
}


#endif //!RST_BASE_CONTROLLER_H
