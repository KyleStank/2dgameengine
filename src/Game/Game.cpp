#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <lua/lua.h>

#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

Game::Game()
{
    _isRunning = false;
    _msPreviousFrame = MS_PER_FRAME;
    registry = new Registry();

    Logger::Log("Game constructor invoked.");
}

Game::~Game()
{
    Logger::Log("Game destructor invoked.");
}

void Game::Initialize()
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

void Game::ProcessInput()
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

void Game::Setup()
{
    Entity testEntity1 = registry->CreateEntity();
    Entity testEntity2 = registry->CreateEntity();
}

void Game::Update()
{
    // TODO: Invoke systems
    // MovementSystem.Update();
    // CollisionSystem.Update();
    // DamageSystem.Update();
}

void Game::Render()
{
    // Draw background.
    SDL_SetRenderDrawColor(_renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    // TODO: Render game objects
    
    // Swap back buffer with front buffer.
    SDL_RenderPresent(_renderer);
}

void Game::EnforceFrameRate()
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

void Game::Run()
{
    Setup();
    while (_isRunning)
    {
        ProcessInput();
        Update();
        Render();
        EnforceFrameRate();
    }
}

void Game::Destroy()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
