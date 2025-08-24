#ifndef SINGLETON_H
#define SINGLETON_H

namespace engine
{
    template<typename singleton_t>
    class Singleton
    {
    public:
        virtual ~Singleton( ) = default;

        Singleton( const Singleton& )                = delete;
        Singleton( Singleton&& ) noexcept            = delete;
        Singleton& operator=( const Singleton& )     = delete;
        Singleton& operator=( Singleton&& ) noexcept = delete;


        static singleton_t& get_instance( )
        {
            static singleton_t instance{};
            return instance;
        }

    protected:
        Singleton( ) = default;

    };
}

#endif //!SINGLETON_H
