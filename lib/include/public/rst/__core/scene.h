#ifndef RST_SCENE_H
#define RST_SCENE_H

#include <rst/pch.h>

#include <rst/type/deleter.h>
#include <rst/__core/gameobject.h>


// TODO: Add documentation
namespace rst
{
    class scene final
    {
    public:
        explicit scene( std::string name );
        ~scene( ) noexcept = default;

        scene( scene const& )                        = delete;
        scene( scene&& ) noexcept                    = delete;
        auto operator=( scene const& ) -> scene&     = delete;
        auto operator=( scene&& ) noexcept -> scene& = delete;

        auto add( std::unique_ptr<gameobject> object ) -> void;
        auto remove( gameobject& object ) -> void;
        auto remove_all( ) -> void;

        // todo: make const correct
        [[nodiscard]] auto find_object(
            std::function<bool( gameobject const& )> const& predicate ) const -> OptionalRef<gameobject>;
        [[nodiscard]] auto create_object( ) -> gameobject&;

        [[nodiscard]] auto name( ) const -> std::string const&;
        [[nodiscard]] auto id( ) const -> uint16_t;

        auto fixed_tick( ) const -> void;
        auto tick( ) const -> void;
        auto render( ) const -> void;

        auto cleanup( ) -> void;

    private:
        static uint16_t s_id_counter_;

        std::string const name_;
        uint16_t const id_{};

        std::vector<std::unique_ptr<gameobject>> objects_{};
        deleter<gameobject> deleter_{};
    };
}


#endif //!RST_SCENE_H
