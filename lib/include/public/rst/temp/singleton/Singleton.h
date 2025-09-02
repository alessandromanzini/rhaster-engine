#ifndef RST_SINGLETON_H
#define RST_SINGLETON_H


namespace rst
{
    template <typename T>
    class Singleton
    {
    public:
        virtual ~Singleton( ) = default;

        Singleton( Singleton const& )                        = delete;
        Singleton( Singleton&& ) noexcept                    = delete;
        auto operator=( Singleton const& ) -> Singleton&     = delete;
        auto operator=( Singleton&& ) noexcept -> Singleton& = delete;


        static auto get_instance( ) -> T&
        {
            static T instance{};
            return instance;
        }

    protected:
        Singleton( ) = default;
    };
}


#endif //!RST_SINGLETON_H
