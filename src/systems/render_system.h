#ifndef ENGINE_RENDERSYSTEM_H
#define ENGINE_RENDERSYSTEM_H

#include <vector>
#include <SDL2/SDL.h>
#include "../ecs.h"
#include "../resources.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"

namespace engine::systems
{
    class render_system: public ecs::system
    {
        public:
            render_system()
            {
                require_component<components::transform_component>();
                require_component<components::sprite_component>();
            }

            void update(SDL_Renderer* renderer)
            {
                for (const ecs::entity entity: get_system_entities())
                {
                    const components::transform_component transform = entity.get_component<components::transform_component>();
                    const components::sprite_component sprite = entity.get_component<components::sprite_component>();

                    SDL_Rect src_rect = sprite.src_rect;
                    SDL_Rect dest_rect = {
                        static_cast<int>(transform.position.x),
                        static_cast<int>(transform.position.y),
                        static_cast<int>(sprite.width * transform.scale.x),
                        static_cast<int>(sprite.height * transform.scale.y)
                    };

                    SDL_RenderCopyEx(
                        renderer,
                        resources::get_texture(sprite.asset_id),
                        &src_rect,
                        &dest_rect,
                        transform.rotation,
                        NULL,
                        SDL_FLIP_NONE
                    );
                }
            }
    };
}

#endif
