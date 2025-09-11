#ifndef RST_BLACKBOARD_H
#define RST_BLACKBOARD_H

#include <rst/pch.h>


namespace rst
{
    namespace internal
    {
        // TODO: add documentation
        // TODO: add inheritance tree hash recognition
        // +---------------------------+
        // | FIELD                     |
        // +---------------------------+
        class base_blackboard_field
        {
        public:
            explicit base_blackboard_field( meta::hash::hash_type const hash ) : type_hash_{ hash } { }

            virtual ~base_blackboard_field( ) = default;

            base_blackboard_field( base_blackboard_field const& )                        = delete;
            base_blackboard_field( base_blackboard_field&& ) noexcept                    = delete;
            auto operator=( base_blackboard_field const& ) -> base_blackboard_field&     = delete;
            auto operator=( base_blackboard_field&& ) noexcept -> base_blackboard_field& = delete;

            [[nodiscard]] auto type_hash( ) const -> meta::hash::hash_type { return type_hash_; }

        private:
            meta::hash::hash_type type_hash_;
        };


        template <typename TField>
        class blackboard_field final : public base_blackboard_field
        {
        public:
            explicit blackboard_field( TField data )
                : base_blackboard_field{ meta::hash::type_hash_v<TField> }
                , data_{ data } { }


            auto data( ) -> TField { return data_; }
            auto set_data( TField data ) -> void { data_ = data; }

        private:
            TField data_;
        };
    }


    // +---------------------------+
    // | BLACKBOARD                |
    // +---------------------------+
    class blackboard final
    {
    public:
        blackboard( )  = default;
        ~blackboard( ) = default;

        blackboard( blackboard const& other )                    = delete;
        auto operator=( blackboard const& other ) -> blackboard& = delete;
        blackboard( blackboard&& other )                         = delete;
        auto operator=( blackboard&& other ) -> blackboard&      = delete;

        /**
         * Add a new field to the blackboard.
         * @tparam TData
         * @param mark
         * @param data Starting data added to the blackboard
         * @return true if the field was added successfully, false if the field already exists
         */
        template <typename TData>
        auto store( earmark mark, TData data ) -> bool;

        /**
         * Change the field of the blackboard.
         * @tparam TData
         * @param mark
         * @param data
         * @return true if the field was changed successfully, false if the field does not exist or type mismatch
         */
        template <typename TData>
        auto edit( earmark mark, TData data ) -> bool;

        /**
         * Get data from the blackboard.
         * @tparam TData
         * @param mark
         * @param data
         * @return true if the data was retrieved successfully, false if the field does not exist or type mismatch
         */
        template <typename TData>
        auto retrieve( earmark mark, TData& data ) const -> bool;

    private:
        std::unordered_map<earmark, std::unique_ptr<internal::base_blackboard_field>> blackboard_data_{};
    };


    template <typename TData>
    auto blackboard::store( earmark const mark, TData data ) -> bool
    {
        if ( blackboard_data_.contains( mark ) )
        {
            return false;
        }
        blackboard_data_.insert( std::make_pair( mark, std::make_unique<internal::blackboard_field<TData>>( data ) ) );
        return true;
    }


    template <typename TData>
    auto blackboard::edit( earmark const mark, TData data ) -> bool
    {
        // existence check
        auto const it = blackboard_data_.find( mark );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // mismatch check
        auto const field = static_cast<internal::blackboard_field<TData>*>( it->second.get( ) );
        if ( field->type_hash( ) != meta::hash::type_hash_v<TData> )
        {
            return false;
        }

        field->set_data( data );
        return true;
    }


    template <typename TData>
    auto blackboard::retrieve( earmark const mark, TData& data ) const -> bool
    {
        // existence check
        auto const it = blackboard_data_.find( mark );
        if ( it == blackboard_data_.end( ) )
        {
            return false;
        }

        // mismatch check
        auto const field = static_cast<internal::blackboard_field<TData>*>( it->second.get( ) );
        if ( field->type_hash( ) != meta::hash::type_hash_v<TData> )
        {
            return false;
        }

        data = field->data( );
        return true;
    }
}


#endif //!RST_BLACKBOARD_H
