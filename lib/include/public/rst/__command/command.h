#ifndef RST_COMMAND_H
#define RST_COMMAND_H


namespace rst
{
    class command
    {
    public:
        command( )          = default;
        virtual ~command( ) = default;

        command( command const& )                        = delete;
        command( command&& ) noexcept                    = delete;
        auto operator=( command const& ) -> command&     = delete;
        auto operator=( command&& ) noexcept -> command& = delete;

        virtual auto execute( ) -> void = 0;
    };
}


#endif //!RST_COMMAND_H
