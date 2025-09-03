#ifndef RST_SINGLETON_H
#define RST_SINGLETON_H


namespace rst
{
    template <typename T>
    class singleton
    {
    public:
        virtual ~singleton( ) = default;

        singleton( singleton const& )                        = delete;
        singleton( singleton&& ) noexcept                    = delete;
        auto operator=( singleton const& ) -> singleton&     = delete;
        auto operator=( singleton&& ) noexcept -> singleton& = delete;


        static auto instance( ) -> T&
        {
            static T instance{};
            return instance;
        }

    protected:
        singleton( ) = default;
    };
}


#endif //!RST_SINGLETON_H
