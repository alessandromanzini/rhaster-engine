#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <framework/GameObject.h>


namespace engine
{
    class BaseController
    {
    public:
        virtual ~BaseController( ) = default;

        BaseController( const BaseController& )                = delete;
        BaseController( BaseController&& ) noexcept            = delete;
        BaseController& operator=( const BaseController& )     = delete;
        BaseController& operator=( BaseController&& ) noexcept = delete;

        /**
         * This method is called when a controller possesses a GameObject.
         * @param pawn The pointer to the possessed GameObject
         */
        virtual void possess( GameObject* pawn );

        /**
         * This method is called when a controller unpossesses a GameObject.
         */
        virtual void unpossess( );

        [[nodiscard]] const GameObject* get_pawn( ) const;
        [[nodiscard]] GameObject* get_pawn( );

        [[nodiscard]] bool has_pawn( ) const;

        [[nodiscard]] int32_t get_id( ) const;

        [[nodiscard]] bool operator==( const BaseController& other ) const;
        [[nodiscard]] bool operator==( const BaseController* other ) const;

    protected:
        const int32_t id_{ -1 };

        BaseController( );

    private:
        inline static int32_t s_id_counter_{ 0 };

        GameObject* pawn_ptr_{ nullptr };

    };


    inline void BaseController::possess( GameObject* pawn )
    {
        assert( pawn && "BaseController::possess: Pawn cannot be nullptr!" );

        if ( pawn_ptr_ )
        {
            unpossess( );
        }
        pawn_ptr_ = pawn;
        pawn_ptr_->on_deletion.bind( this, &BaseController::unpossess );
    }


    inline void BaseController::unpossess( )
    {
        pawn_ptr_->on_deletion.unbind( this );
        pawn_ptr_ = nullptr;
    }


    inline const GameObject* BaseController::get_pawn( ) const
    {
        return pawn_ptr_;
    }


    inline GameObject* BaseController::get_pawn( )
    {
        return pawn_ptr_;
    }


    inline bool BaseController::has_pawn( ) const
    {
        return pawn_ptr_ != nullptr;
    }


    inline int32_t BaseController::get_id( ) const
    {
        return id_;
    }


    inline bool BaseController::operator==( const BaseController& other ) const
    {
        return id_ == other.id_;
    }


    inline bool BaseController::operator==( const BaseController* other ) const
    {
        return id_ == other->id_;
    }


    inline BaseController::BaseController( ) : id_( s_id_counter_++ ) { }

}


#endif //!BASECONTROLLER_H
