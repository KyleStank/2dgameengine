#ifndef ENGINE_RESOURCES_H
#define ENGINE_RESOURCES_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

namespace engine::resources
{
    struct asset
    {
        public:
            std::string id;
            std::string name;
            std::string path;
    };

    struct texture_asset: public asset
    {
        SDL_Texture* texture;

        texture_asset(std::string id, std::string path)
        {
            this->id = this->name = id;
            this->path = path;
            this->texture = nullptr;
        }

        texture_asset(std:: string id, std::string path, SDL_Texture* texture)
        {
            texture_asset(id, path);
            this->texture = texture;
        }
    };

    void clear_textures();
    void load_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path);
    SDL_Texture* get_texture(const std::string& asset_id);
}

#endif
