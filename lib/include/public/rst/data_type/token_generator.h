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


        [[nodiscard]] auto generate( ) -> TToken
        {
            if ( next_token_ == std::numeric_limits<TToken>::max( ) )
            {
                throw std::runtime_error{ "token_generator::generate: token overflow!" };
            }
            return next_token_++;
        }


        auto reset( ) noexcept -> void { next_token_ = first_token; }

    private:
        static constexpr TToken first_token{ invalid_token + 1U };
        TToken next_token_{ first_token };
    };
}


#endif //!RST_TOKEN_GENERATOR_H
