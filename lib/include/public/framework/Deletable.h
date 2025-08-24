#ifndef DELETABLE_H
#define DELETABLE_H


namespace engine
{
    class Deletable
    {
    public:
        Deletable( ) = default;
        virtual ~Deletable( ) = default;

        Deletable( const Deletable& )                = delete;
        Deletable( Deletable&& ) noexcept            = delete;
        Deletable& operator=( const Deletable& )     = delete;
        Deletable& operator=( Deletable&& ) noexcept = delete;

        virtual void mark_for_deletion( ) = 0;

    };

}


#endif //!DELETABLE_H
