#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#include <SDL2/SDL.h>
#include "../ecs/ecs.h"
#include "../resources/resources.h"

namespace engine
{
    const int FPS = 60;
    const int MS_PER_FRAME = 1000 / FPS;

    class Game
    {
        private:
            bool _isRunning;
            Uint32 _msPreviousFrame;
            double _deltaTime;

            SDL_Window* _window;
            SDL_Renderer* _renderer;

            std::unique_ptr<Registry> _registry;
            std::unique_ptr<Resources> _resources;

        public:
            Game();
            ~Game();

            int windowWidth;
            int windowHeight;

            void Initialize();

            void Run();
            void LoadLevel(int number);
            void Setup();
            void ProcessInput();
            void EnforceFrameRate();
            void Update();
            void Render();

            void Destroy();
    };
}

#endif
