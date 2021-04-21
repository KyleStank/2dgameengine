#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Resources/Resources.h"

class RenderSystem: public System
{
    public:
        RenderSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<Resources>& resources)
        {
            for (const Entity entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect destRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };

                SDL_RenderCopyEx(
                    renderer,
                    resources->GetTexture(sprite.assetId),
                    &srcRect,
                    &destRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                );

                // SDL_Rect rect = {
                //     static_cast<int>(transform.position.x),
                //     static_cast<int>(transform.position.y),
                //     sprite.width,
                //     sprite.height
                // };

                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // SDL_RenderFillRect(renderer, &rect);
            }
        }
};

#endif
