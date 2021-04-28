#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <lua/lua.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "../io.h"
#include "../util.h"
#include "../components/rigidbody_component.h"
#include "../components/transform_component.h"
#include "../components/sprite_component.h"
#include "../ecs/ecs.h"
#include "../logger/logger.h"
#include "../systems/render_system.h"
#include "../systems/movement_system.h"

#include <iostream>
engine::Game::Game()
{
    _isRunning = false;
    _msPreviousFrame = MS_PER_FRAME;

    _registry = std::make_unique<Registry>();
    _resources = std::make_unique<Resources>();

    Logger::Log("Game constructor invoked.");
}

engine::Game::~Game()
{
    Logger::Log("Game destructor invoked.");
}

void engine::Game::Initialize()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Error("Error initializing SDL.");
        return;
    }

    Logger::Log("SDL Initialized.");

    // TODO: Set this dynamically a better way? We don't want each user to see different amount of space.
    // Get height and width of display and create window.
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    {
        Logger::Error("Error retrieving display mode.");
        return;
    }

    Logger::Log("Display mode retrieved.");

    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    // windowWidth = 1920;
    // windowHeight = 1080;
    _window = SDL_CreateWindow(
        "Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WindowFlags::SDL_WINDOW_FULLSCREEN
    );
    if (!_window)
    {
        Logger::Error("Error creating SDL window.");
        return;
    }

    Logger::Log("Window created.");

    // TODO: Put resize function somewhere. Useful for changing resolutions on a settings page or something.
    // int top;
    // SDL_GetWindowBordersSize(_window, &top, NULL, NULL, NULL); // Grab borders size.
    // SDL_SetWindowSize(_window, displayMode.w, displayMode.h - top); // Set new position, accounting for borders.
    // SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // Recenter window.

    // Create renderer.
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (!_renderer)
    {
        Logger::Error("Error creating SDL renderer.");
        return;
    }

    Logger::Log("Renderer created.");

    _isRunning = true;
}

void engine::Game::Run()
{
    LoadLevel(1);
    Setup();
    while (_isRunning)
    {
        ProcessInput();
        EnforceFrameRate();
        Update();
        Render();
    }
}

void engine::Game::LoadLevel(int level)
{
    // Load textures from Resources.
    _resources->LoadTexture(_renderer, "jungle-tileset", "./assets/tilemaps/jungle.png");
    _resources->LoadTexture(_renderer, "tank-image", "./assets/images/tank-panther-right.png");
    _resources->LoadTexture(_renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Add systems.
    _registry->AddSystem<RenderSystem>();
    _registry->AddSystem<MovementSystem>();

    // Setup tank entity.
    Entity tankEntity = _registry->CreateEntity();
    tankEntity.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tankEntity.AddComponent<RigidbodyComponent>(glm::vec2(50.0, 0.0));
    tankEntity.AddComponent<SpriteComponent>("tank-image", 32, 32);

    // Setup truck entity.
    Entity truckEntity = _registry->CreateEntity();
    truckEntity.AddComponent<TransformComponent>(glm::vec2(10.0, 50.0), glm::vec2(2.0, 2.0), 90.0);
    truckEntity.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 20.0));
    truckEntity.AddComponent<SpriteComponent>("truck-image", 32, 32);
}

void engine::Game::Setup()
{

}

void engine::Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_EventType::SDL_QUIT:
                _isRunning = false;
                break;

            case SDL_EventType::SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDL_KeyCode::SDLK_ESCAPE)
                {
                    _isRunning = false;
                }
                break;
        }
    }
}

void engine::Game::EnforceFrameRate()
{
    // If we are running too fast, waste some time until we reach MS_PER_FRAME.
    Uint32 timeToWait = MS_PER_FRAME - (SDL_GetTicks() - _msPreviousFrame);
    if (timeToWait > 0 && timeToWait <= _msPreviousFrame)
    {
        SDL_Delay(timeToWait);
    }

    // Calculate the number of seconds since the last frame.
    _deltaTime = (SDL_GetTicks() - _msPreviousFrame) / 1000.0;

    // Store "previous" frame time.
    _msPreviousFrame = SDL_GetTicks();
}

void engine::Game::Update()
{
    // Update systems.
    _registry->GetSystem<MovementSystem>().Update(_deltaTime);

    // Update registry to process pending entities.
    _registry->Update();
}

void engine::Game::Render()
{
    // Draw background.
    SDL_SetRenderDrawColor(_renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _resources);

    // Swap back buffer with front buffer.
    SDL_RenderPresent(_renderer);
}

void engine::Game::Destroy()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
