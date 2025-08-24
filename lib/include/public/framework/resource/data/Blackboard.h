#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <core/UID.h>

#include <algorithm>
#include <memory>


namespace engine
{
    // +---------------------------+
    // | FIELD			           |
    // +---------------------------+
    class IBlackboardField
    {
    public:
        explicit IBlackboardField( const type_utility::hash_value_t hash ) : type_hash_{ hash } { }

        virtual ~IBlackboardField( ) = default;

        IBlackboardField( const IBlackboardField& )                = delete;
        IBlackboardField( IBlackboardField&& ) noexcept            = delete;
        IBlackboardField& operator=( const IBlackboardField& )     = delete;
        IBlackboardField& operator=( IBlackboardField&& ) noexcept = delete;

        [[nodiscard]] type_utility::hash_value_t get_type_hash( ) const { return type_hash_; }

    private:
        type_utility::hash_value_t type_hash_;

    };

    template <typename field_t>
    class BlackboardField final : public IBlackboardField
    {
    public:
        explicit BlackboardField( field_t data )
            : IBlackboardField( type_utility::type_hash<field_t>( ) )
            , data_{ data } { }


        field_t get_data( ) { return data_; }
        void set_data( field_t data ) { data_ = data; }

    private:
        field_t data_;

    };


    // +---------------------------+
    // | BLACKBOARD		           |
    // +---------------------------+
    class Blackboard final
    {
    public:
        Blackboard( )  = default;
        ~Blackboard( ) = default;

        Blackboard( const Blackboard& other )            = delete;
        Blackboard& operator=( const Blackboard& other ) = delete;
        Blackboard( Blackboard&& other )                 = delete;
        Blackboard& operator=( Blackboard&& other )      = delete;

        /**
         * Add a new field to the blackboard.
         * @tparam data_t
         * @param uid
         * @param data Starting data added to the blackboard
         * @return true if the field was added successfully, false if the field already exists
         */
        template <typename data_t>
        bool store( UID uid, data_t data );

        /**
         * Change the field of the blackboard.
         * @tparam data_t
         * @param uid
         * @param data
         * @return true if the field was changed successfully, false if the field does not exist or type mismatch
         */
        template <typename data_t>
        bool edit( UID uid, data_t data );

        /**
         * Get data from the blackboard.
         * @tparam data_t
         * @param uid
         * @param data
         * @return true if the data was retrieved successfully, false if the field does not exist or type mismatch
         */
        template <typename data_t>
        bool retrieve( UID uid, data_t& data ) const;

    private:
        std::unordered_map<UID, std::unique_ptr<IBlackboardField>> blackboard_data_{};
    };


    template <typename data_t>
    bool Blackboard::store( const UID uid, data_t data )
    {
        if ( blackboard_data_.contains( uid ) )
        {
            return false;
        }
        blackboard_data_.insert( std::make_pair( uid, std::make_unique<BlackboardField<data_t>>( data ) ) );
        return true;
    }


    template <typename data_t>
    bool Blackboard::edit( const UID uid, data_t data )
    {
        // Existence check
        const auto it = blackboard_data_.find( uid );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // Mismatch check
        const auto field = static_cast<BlackboardField<data_t>*>( it->second.get( ) );
        if ( field->get_type_hash( ) != type_utility::type_hash<data_t>( ) )
        {
            return false;
        }

        field->set_data(  data );
        return true;
    }


    template <typename data_t>
    bool Blackboard::retrieve( const UID uid, data_t& data ) const
    {
        // Existence check
        const auto it = blackboard_data_.find( uid );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // Mismatch check
        const auto field = static_cast<BlackboardField<data_t>*>( it->second.get( ) );
        if ( field->get_type_hash( ) != type_utility::type_hash<data_t>( ) )
        {
            return false;
        }

        data = field->get_data( );
        return true;
    }

}


#endif //!BLACKBOARD_H
