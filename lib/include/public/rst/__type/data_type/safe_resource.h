#ifndef RST_SAFE_RESOURCE_H
#define RST_SAFE_RESOURCE_H

#include <rst/pch.h>


// TODO: add documentation
namespace rst::thread
{
    template <template<typename> class TLock, typename TResource> requires
        std::is_same_v<TLock<std::mutex>, std::lock_guard<std::mutex>> || std::is_same_v<TLock<std::mutex>, std::unique_lock<
            std::mutex>>
    struct ResourceLock
    {
        explicit ResourceLock( std::mutex& mutex, TResource& resource )
            : lock{ mutex }
            , resource{ resource } { }


        TLock<std::mutex> lock;
        TResource& resource;
    };


    template <typename TResource>
    class SafeResource
    {
    public:
        using resource_t = TResource;

        explicit SafeResource( TResource&& resource );
        ~SafeResource( ) = default;

        SafeResource( SafeResource const& )                        = delete;
        SafeResource( SafeResource&& ) noexcept                    = delete;
        auto operator=( SafeResource const& ) -> SafeResource&     = delete;
        auto operator=( SafeResource&& ) noexcept -> SafeResource& = delete;

        [[nodiscard]] auto get( ) -> ResourceLock<std::lock_guard, TResource>;
        [[nodiscard]] auto get_unique( ) -> ResourceLock<std::unique_lock, TResource>;
        [[nodiscard]] auto cget( ) const -> TResource const&;

        auto set( TResource const& value ) -> void;
        auto set( TResource&& sink ) -> void;

        [[nodiscard]] auto clone( ) const -> TResource;

    private:
        std::mutex resource_guard_{};
        TResource resource_;
    };


    template <typename TResource>
    SafeResource<TResource>::SafeResource( TResource&& resource ) : resource_{ std::move( resource ) } { }


    template <typename TResource>
    auto SafeResource<TResource>::get( ) -> ResourceLock<std::lock_guard, TResource>
    {
        return ResourceLock<std::lock_guard, resource_t>{ resource_guard_, resource_ };
    }


    template <typename TResource>
    auto SafeResource<TResource>::get_unique( ) -> ResourceLock<std::unique_lock, TResource>
    {
        return ResourceLock<std::unique_lock, resource_t>{ resource_guard_, resource_ };
    }


    template <typename TResource>
    auto SafeResource<TResource>::cget( ) const -> TResource const&
    {
        return resource_;
    }


    template <typename TResource>
    auto SafeResource<TResource>::set( TResource const& value ) -> void
    {
        assert(
            std::is_copy_assignable_v<TResource> &&
            "SafeResource::set( const resource_t& ) requires copy-assignable resource_t!" );

        std::lock_guard lock{ resource_guard_ };
        resource_ = value;
    }


    template <typename TResource>
    auto SafeResource<TResource>::set( TResource&& sink ) -> void
    {
        assert(
            std::is_move_assignable_v<TResource> &&
            "SafeResource::set( const resource_t& ) requires move-assignable resource_t!" );

        std::lock_guard lock( resource_guard_ );
        resource_ = std::move( sink );
    }


    template <typename TResource>
    auto SafeResource<TResource>::clone( ) const -> TResource
    {
        assert( std::is_copy_constructible_v<TResource> && "SafeResource::clone() requires copy-constructible resource_t!" );
        return resource_;
    }
}


#endif //!RST_SAFE_RESOURCE_H
