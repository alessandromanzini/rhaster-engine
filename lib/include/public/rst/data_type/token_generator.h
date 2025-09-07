#ifndef RST_TOKEN_GENERATOR_H
#define RST_TOKEN_GENERATOR_H


namespace rst
{
    template <typename TToken, TToken invalid_token>
    class token_generator final
    {
    public:
        token_generator( ) noexcept  = default;
        ~token_generator( ) noexcept = default;

        token_generator( token_generator const& )                        = delete;
        token_generator( token_generator&& ) noexcept                    = delete;
        auto operator=( token_generator const& ) -> token_generator&     = delete;
        auto operator=( token_generator&& ) noexcept -> token_generator& = delete;


        /**
         * @return A sequentially unique token, or std::nullopt if the maximum token value has been reached.
         * @complexity O(1)
         */
        [[nodiscard]] auto generate( ) noexcept -> TToken
        {
            if ( is_saturated( ) )
            {
                return invalid_token;
            }
            return next_token_++;
        }


        /**
         * @return True if the generator has reached the maximum token value and can no longer issue new tokens.
         */
        [[nodiscard]] auto is_saturated( ) const noexcept -> bool { return next_token_ == std::numeric_limits<TToken>::max( ); }


        /**
         * @brief Resets the token generator to start issuing tokens from the beginning.
         */
        auto reset( ) noexcept -> void { next_token_ = first_token; }

    private:
        static constexpr TToken first_token{ invalid_token + 1U };
        TToken next_token_{ first_token };
    };
}


#endif //!RST_TOKEN_GENERATOR_H
