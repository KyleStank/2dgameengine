#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "logger.h"
#include "resources.h"

std::map<std::string, SDL_Texture*> _textures;

// Clears all of the loaded textures from memory.
void engine::resources::clear_textures()
{
    for (std::pair<std::string, SDL_Texture*> pair: _textures)
    {
        SDL_DestroyTexture(pair.second);
    }

    _textures.clear();
}

// Loads a texture into memory.
void engine::resources::load_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    _textures.emplace(asset_id, texture);

    logger::log("Texture[" + asset_id + "] loaded in memory from [" + path + "].");
}

// Retrieves the pointer to a texture loaded in memory.
SDL_Texture* engine::resources::get_texture(const std::string& asset_id)
{
    return _textures[asset_id];
}
