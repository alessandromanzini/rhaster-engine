#ifndef RST_SCENE_H
#define RST_SCENE_H

#include <rst/pch.h>

#include <rst/framework/data_type.h>
#include <rst/__core/gameobject.h>


// TODO: Add documentation
namespace rst
{
    class Scene final
    {
    public:
        explicit Scene( std::string name );
        ~Scene( ) noexcept = default;

        Scene( Scene const& )                        = delete;
        Scene( Scene&& ) noexcept                    = delete;
        auto operator=( Scene const& ) -> Scene&     = delete;
        auto operator=( Scene&& ) noexcept -> Scene& = delete;

        auto add( std::unique_ptr<GameObject> object ) -> void;
        auto remove( GameObject& object ) -> void;
        auto remove_all( ) -> void;

        // todo: make const correct
        [[nodiscard]] auto find_object(
            std::function<bool( GameObject const& )> const& predicate ) const -> OptionalRef<GameObject>;
        [[nodiscard]] auto create_object( ) -> GameObject&;

        [[nodiscard]] auto get_name( ) const -> std::string const&;
        [[nodiscard]] auto get_id( ) const -> uint16_t;

        auto fixed_tick( ) const -> void;
        auto tick( ) const -> void;
        auto render( ) const -> void;

        auto cleanup( ) -> void;

    private:
        static uint16_t s_id_counter_;

        std::string const name_;
        uint16_t const id_{};

        std::vector<std::unique_ptr<GameObject>> objects_{};
        Deleter<GameObject> deleter_{};
    };
}


#endif //!RST_SCENE_H
