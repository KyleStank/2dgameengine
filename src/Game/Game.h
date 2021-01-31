#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

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
        
        Registry* registry;
    
    public:
        Game();
        ~Game();

        int windowWidth;
        int windowHeight;

        void Initialize();
        void Setup();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void EnforceFrameRate();
        void Destroy();
};

#endif
