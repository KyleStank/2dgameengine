#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <vector>

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem: public System
{
    public:
        RenderSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer)
        {
            for (const Entity entity: GetSystemEntities())
            {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

                SDL_Rect rect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    sprite.width,
                    sprite.height
                };

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
};

#endif
