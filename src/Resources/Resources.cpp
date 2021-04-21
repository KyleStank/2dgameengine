#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Resources.h"
#include "../Logger/Logger.h"

Resources::Resources()
{
    Logger::Log("Resources constructor invoked.");
}

Resources::~Resources()
{
    ClearTextures();
    Logger::Log("Resources destructor invoked.");
}

void Resources::ClearTextures()
{
    for (std::pair<std::string, SDL_Texture*> pair: textures)
    {
        SDL_DestroyTexture(pair.second);
    }

    textures.clear();
}

void Resources::LoadTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);

    Logger::Log("Texture[" + assetId + "] loaded in memory from [" + path + "].");
}

SDL_Texture* Resources::GetTexture(const std::string& assetId)
{
    return textures[assetId];
}