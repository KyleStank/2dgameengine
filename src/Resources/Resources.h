#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

struct IAsset
{
    public:
        std::string id;
        std::string name;
        std::string path;
};

struct TextureAssets: public IAsset
{
    SDL_Texture* texture;

    TextureAssets(std::string id, std::string path)
    {
        this->id = this->name = id;
        this->path = path;
        this->texture = nullptr;
    }

    TextureAssets(std:: string id, std::string path, SDL_Texture* texture)
    {
        TextureAssets(id, path);
        this->texture = texture;
    }
};

class Resources
{
    private:
        std::map<std::string, SDL_Texture*> textures;
    
    public:
        Resources();
        ~Resources();

        void ClearTextures();
        void LoadTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
        SDL_Texture* GetTexture(const std::string& assetId);
};

#endif
