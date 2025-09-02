#ifndef RST_BLACKBOARD_H
#define RST_BLACKBOARD_H

#include <rst/pch.h>


namespace rst
{
    // TODO: add documentation
    // +---------------------------+
    // | FIELD                     |
    // +---------------------------+
    class IBlackboardField
    {
    public:
        explicit IBlackboardField( meta::hash::hash_value_t const hash ) : type_hash_{ hash } { }

        virtual ~IBlackboardField( ) = default;

        IBlackboardField( IBlackboardField const& )                        = delete;
        IBlackboardField( IBlackboardField&& ) noexcept                    = delete;
        auto operator=( IBlackboardField const& ) -> IBlackboardField&     = delete;
        auto operator=( IBlackboardField&& ) noexcept -> IBlackboardField& = delete;

        [[nodiscard]] auto get_type_hash( ) const -> meta::hash::hash_value_t { return type_hash_; }

    private:
        meta::hash::hash_value_t type_hash_;
    };


    template <typename field_t>
    class BlackboardField final : public IBlackboardField
    {
    public:
        explicit BlackboardField( field_t data )
            : IBlackboardField( meta::hash::type_hash<field_t>( ) )
            , data_{ data } { }


        auto get_data( ) -> field_t { return data_; }
        auto set_data( field_t data ) -> void { data_ = data; }

    private:
        field_t data_;
    };


    // +---------------------------+
    // | BLACKBOARD                |
    // +---------------------------+
    class Blackboard final
    {
    public:
        Blackboard( )  = default;
        ~Blackboard( ) = default;

        Blackboard( Blackboard const& other )                    = delete;
        auto operator=( Blackboard const& other ) -> Blackboard& = delete;
        Blackboard( Blackboard&& other )                         = delete;
        auto operator=( Blackboard&& other ) -> Blackboard&      = delete;

        /**
         * Add a new field to the blackboard.
         * @tparam data_t
         * @param uid
         * @param data Starting data added to the blackboard
         * @return true if the field was added successfully, false if the field already exists
         */
        template <typename data_t>
        auto store( UID uid, data_t data ) -> bool;

        /**
         * Change the field of the blackboard.
         * @tparam data_t
         * @param uid
         * @param data
         * @return true if the field was changed successfully, false if the field does not exist or type mismatch
         */
        template <typename data_t>
        auto edit( UID uid, data_t data ) -> bool;

        /**
         * Get data from the blackboard.
         * @tparam data_t
         * @param uid
         * @param data
         * @return true if the data was retrieved successfully, false if the field does not exist or type mismatch
         */
        template <typename data_t>
        auto retrieve( UID uid, data_t& data ) const -> bool;

    private:
        std::unordered_map<UID, std::unique_ptr<IBlackboardField>> blackboard_data_{};
    };


    template <typename data_t>
    auto Blackboard::store( UID const uid, data_t data ) -> bool
    {
        if ( blackboard_data_.contains( uid ) )
        {
            return false;
        }
        blackboard_data_.insert( std::make_pair( uid, std::make_unique<BlackboardField<data_t>>( data ) ) );
        return true;
    }


    template <typename data_t>
    auto Blackboard::edit( UID const uid, data_t data ) -> bool
    {
        // existence check
        auto const it = blackboard_data_.find( uid );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // mismatch check
        auto const field = static_cast<BlackboardField<data_t>*>( it->second.get( ) );
        if ( field->get_type_hash( ) != meta::hash::type_hash<data_t>( ) )
        {
            return false;
        }

        field->set_data( data );
        return true;
    }


    template <typename data_t>
    auto Blackboard::retrieve( UID const uid, data_t& data ) const -> bool
    {
        // existence check
        auto const it = blackboard_data_.find( uid );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // mismatch check
        auto const field = static_cast<BlackboardField<data_t>*>( it->second.get( ) );
        if ( field->get_type_hash( ) != meta::hash::type_hash<data_t>( ) )
        {
            return false;
        }

        data = field->get_data( );
        return true;
    }
}


#endif //!RST_BLACKBOARD_H
