#ifndef SAFERESOURCE_H
#define SAFERESOURCE_H

#include <algorithm>
#include <cassert>
#include <mutex>


namespace engine::threading
{
    template <template<typename> class lock_t, typename resource_t>
        requires std::is_same_v<lock_t<std::mutex>, std::lock_guard<std::mutex>> ||
                 std::is_same_v<lock_t<std::mutex>, std::unique_lock<std::mutex>>
    struct ResourceLock
    {
        explicit ResourceLock( std::mutex& mutex, resource_t& resource )
            : lock{ mutex }
            , resource{ resource } { }


        lock_t<std::mutex> lock;
        resource_t& resource;
    };

    template <typename _resource_t>
    class SafeResource
    {
    public:
        using resource_t = _resource_t;

        explicit SafeResource( resource_t&& resource );
        ~SafeResource( ) = default;

        SafeResource( const SafeResource& )                = delete;
        SafeResource( SafeResource&& ) noexcept            = delete;
        SafeResource& operator=( const SafeResource& )     = delete;
        SafeResource& operator=( SafeResource&& ) noexcept = delete;

        [[nodiscard]] ResourceLock<std::lock_guard, resource_t> get( );
        [[nodiscard]] ResourceLock<std::unique_lock, resource_t> get_unique( );
        [[nodiscard]] const resource_t& cget( ) const;

        void set( const resource_t& value );
        void set( resource_t&& sink );

        [[nodiscard]] resource_t clone( ) const;

    private:
        std::mutex resource_guard_{};
        resource_t resource_;

    };


    template <typename resource_t>
    SafeResource<resource_t>::SafeResource( resource_t&& resource )
        : resource_( std::move( resource ) ) { }


    template <typename resource_t>
    ResourceLock<std::lock_guard, resource_t> SafeResource<resource_t>::get( )
    {
        return ResourceLock<std::lock_guard, resource_t>{ resource_guard_, resource_ };
    }


    template <typename resource_t>
    ResourceLock<std::unique_lock, resource_t> SafeResource<resource_t>::get_unique( )
    {
        return ResourceLock<std::unique_lock, resource_t>{ resource_guard_, resource_ };
    }


    template <typename resource_t>
    const resource_t& SafeResource<resource_t>::cget( ) const
    {
        return resource_;
    }


    template <typename resource_t>
    void SafeResource<resource_t>::set( const resource_t& value )
    {
        assert( std::is_copy_assignable_v<resource_t> &&
            "SafeResource::set( const resource_t& ) requires copy-assignable resource_t!" );

        std::lock_guard lock( resource_guard_ );
        resource_ = value;
    }


    template <typename resource_t>
    void SafeResource<resource_t>::set( resource_t&& sink )
    {
        assert(
            std::is_move_assignable_v<resource_t> &&
            "SafeResource::set( const resource_t& ) requires move-assignable resource_t!" );

        std::lock_guard lock( resource_guard_ );
        resource_ = std::move( sink );
    }


    template <typename resource_t>
    resource_t SafeResource<resource_t>::clone( ) const
    {
        assert( std::is_copy_constructible_v<resource_t> && "SafeResource::clone() requires copy-constructible resource_t!" );
        return resource_;
    }

}


#endif //!SAFERESOURCE_H
