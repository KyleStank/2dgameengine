#ifndef ENGINE_SPRITECOMPONENT_H
#define ENGINE_SPRITECOMPONENT_H

#include <string>
#include <SDL2/SDL.h>

namespace engine
{
    struct SpriteComponent
    {
        std::string assetId;
        int width;
        int height;
        SDL_Rect srcRect;

        SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0)
        {
            this->assetId = assetId;
            this->width = width;
            this->height = height;
            this->srcRect =
            {
                srcRectX,
                srcRectY,
                width,
                height
            };
        }
    };
}

#endif
