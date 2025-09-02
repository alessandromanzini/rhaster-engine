#ifndef RST_DELETABLE_H
#define RST_DELETABLE_H


namespace rst
{
    class Deletable
    {
    public:
        Deletable( ) = default;
        virtual ~Deletable( ) = default;

        Deletable( Deletable const& )                         = delete;
        Deletable( Deletable&& ) noexcept                     = delete;
        auto operator=( Deletable const& ) -> Deletable&      = delete;
        auto operator=( Deletable&& ) noexcept -> Deletable&  = delete;

        virtual auto mark_for_deletion( ) -> void = 0;

    };

}


#endif //!RST_DELETABLE_H
