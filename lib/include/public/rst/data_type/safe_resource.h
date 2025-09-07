#ifndef RST_SAFE_RESOURCE_H
#define RST_SAFE_RESOURCE_H

#include <rst/pch.h>


// TODO: add documentation
namespace rst::thread
{
    namespace internal
    {
        using std::mutex;
        template <template<typename> class TLock> concept lockable = requires( )
        {
            std::same_as<TLock<mutex>, std::lock_guard<mutex>> || std::same_as<TLock<mutex>, std::unique_lock<mutex>>;
        };
    }


    template <template<typename> class TLock, typename TResource> requires internal::lockable<TLock>
    struct resource_lock
    {
        explicit resource_lock( std::mutex& mutex, TResource& resource )
            : lock{ mutex }
            , resource{ resource } { }


        TLock<std::mutex> lock;
        TResource& resource;
    };


    template <typename TResource>
    class safe_resource
    {
    public:
        using resource_t = TResource;

        explicit safe_resource( TResource&& resource );
        ~safe_resource( ) = default;

        safe_resource( safe_resource const& )                        = delete;
        safe_resource( safe_resource&& ) noexcept                    = delete;
        auto operator=( safe_resource const& ) -> safe_resource&     = delete;
        auto operator=( safe_resource&& ) noexcept -> safe_resource& = delete;

        [[nodiscard]] auto get( ) -> resource_lock<std::lock_guard, TResource>;
        [[nodiscard]] auto unique( ) -> resource_lock<std::unique_lock, TResource>;
        [[nodiscard]] auto cget( ) const -> TResource const&;

        auto set( TResource const& value ) -> void;
        auto set( TResource&& sink ) -> void;

        [[nodiscard]] auto clone( ) const -> TResource;

    private:
        std::mutex resource_guard_{};
        TResource resource_;
    };


    template <typename TResource>
    safe_resource<TResource>::safe_resource( TResource&& resource ) : resource_{ std::move( resource ) } { }


    template <typename TResource>
    auto safe_resource<TResource>::get( ) -> resource_lock<std::lock_guard, TResource>
    {
        return resource_lock<std::lock_guard, resource_t>{ resource_guard_, resource_ };
    }


    template <typename TResource>
    auto safe_resource<TResource>::unique( ) -> resource_lock<std::unique_lock, TResource>
    {
        return resource_lock<std::unique_lock, resource_t>{ resource_guard_, resource_ };
    }


    template <typename TResource>
    auto safe_resource<TResource>::cget( ) const -> TResource const&
    {
        return resource_;
    }


    template <typename TResource>
    auto safe_resource<TResource>::set( TResource const& value ) -> void
    {
        assert(
            std::is_copy_assignable_v<TResource> &&
            "SafeResource::set( const resource_t& ) requires copy-assignable resource_t!" );

        std::lock_guard lock{ resource_guard_ };
        resource_ = value;
    }


    template <typename TResource>
    auto safe_resource<TResource>::set( TResource&& sink ) -> void
    {
        assert(
            std::is_move_assignable_v<TResource> &&
            "SafeResource::set( const resource_t& ) requires move-assignable resource_t!" );

        std::lock_guard lock( resource_guard_ );
        resource_ = std::move( sink );
    }


    template <typename TResource>
    auto safe_resource<TResource>::clone( ) const -> TResource
    {
        assert( std::is_copy_constructible_v<TResource> && "SafeResource::clone() requires copy-constructible resource_t!" );
        return resource_;
    }
}


#endif //!RST_SAFE_RESOURCE_H
