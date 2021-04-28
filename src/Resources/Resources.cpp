#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "resources.h"
#include "../logger/logger.h"

engine::Resources::Resources()
{
    Logger::Log("Resources constructor invoked.");
}

engine::Resources::~Resources()
{
    ClearTextures();
    Logger::Log("Resources destructor invoked.");
}

void engine::Resources::ClearTextures()
{
    for (std::pair<std::string, SDL_Texture*> pair: textures)
    {
        SDL_DestroyTexture(pair.second);
    }

    textures.clear();
}

void engine::Resources::LoadTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);

    Logger::Log("Texture[" + assetId + "] loaded in memory from [" + path + "].");
}

SDL_Texture* engine::Resources::GetTexture(const std::string& assetId)
{
    return textures[assetId];
}
