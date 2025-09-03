#ifndef RST_DELETABLE_H
#define RST_DELETABLE_H


namespace rst
{
    class deletable
    {
    public:
        deletable( ) = default;
        virtual ~deletable( ) = default;

        deletable( deletable const& )                         = delete;
        deletable( deletable&& ) noexcept                     = delete;
        auto operator=( deletable const& ) -> deletable&      = delete;
        auto operator=( deletable&& ) noexcept -> deletable&  = delete;

        virtual auto mark_for_deletion( ) -> void = 0;

    };

}


#endif //!RST_DELETABLE_H
