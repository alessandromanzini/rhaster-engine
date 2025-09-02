#ifndef RST_COMMAND_H
#define RST_COMMAND_H


namespace rst
{
    class Command
    {
    public:
        Command( )          = default;
        virtual ~Command( ) = default;

        Command( Command const& )                        = delete;
        Command( Command&& ) noexcept                    = delete;
        auto operator=( Command const& ) -> Command&     = delete;
        auto operator=( Command&& ) noexcept -> Command& = delete;

        virtual auto execute( ) -> void = 0;
    };
}


#endif //!RST_COMMAND_H
