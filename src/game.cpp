#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <lua/lua.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ecs.h"
#include "game.h"
#include "io.h"
#include "logger.h"
#include "resources.h"
#include "util.h"
#include "./components/rigidbody_component.h"
#include "./components/transform_component.h"
#include "./components/sprite_component.h"
#include "./systems/render_system.h"
#include "./systems/movement_system.h"

engine::game::game()
{
    target_fps = 60;
    _is_running = false;
    _ms_prev_frame = get_ms_per_frame();

    _registry = std::make_unique<ecs::registry>();

    logger::log("Game constructor invoked.");
}

engine::game::~game()
{
    logger::log("Game destructor invoked.");
}

int engine::game::get_ms_per_frame()
{
    return 1000 / target_fps;
}

void engine::game::initialize()
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logger::error("Error initializing SDL.");
        return;
    }

    logger::log("SDL Initialized.");

    // TODO: Set this dynamically a better way? We don't want each user to see different amount of space.
    // Get height and width of display and create window.
    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
    {
        logger::error("Error retrieving display mode.");
        return;
    }

    logger::log("Display mode retrieved.");

    window_width = display_mode.w;
    window_height = display_mode.h;
    // window_width = 1920;
    // window_height = 1080;
    _window = SDL_CreateWindow(
        "2dgameengine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WindowFlags::SDL_WINDOW_FULLSCREEN
    );
    if (!_window)
    {
        logger::error("Error creating SDL window.");
        return;
    }

    logger::log("Window created.");

    // TODO: Put resize function somewhere. Useful for changing resolutions on a settings page or something.
    // int top;
    // SDL_GetWindowBordersSize(_window, &top, NULL, NULL, NULL); // Grab borders size.
    // SDL_SetWindowSize(_window, displayMode.w, displayMode.h - top); // Set new position, accounting for borders.
    // SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // Recenter window.

    // Create renderer.
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    if (!_renderer)
    {
        logger::error("Error creating SDL renderer.");
        return;
    }

    logger::log("Renderer created.");

    _is_running = true;
}

void engine::game::run()
{
    load_level(1);
    setup();
    while (_is_running)
    {
        process_input();
        enforce_frame_rate();
        update();
        render();
    }
}

void engine::game::load_level(int level)
{
    // Load textures from Resources.
    resources::load_texture(_renderer, "jungle-tileset", "./assets/tilemaps/jungle.png");
    resources::load_texture(_renderer, "tank-image", "./assets/images/tank-panther-right.png");
    resources::load_texture(_renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Add systems.
    _registry->add_system<systems::render_system>();
    _registry->add_system<systems::movement_system>();

    // Generate tile map.
    std::string path = "./assets/tilemaps/jungle.map";
    std::vector<std::string> contents = io::read_all_lines(path);
    int contents_size = static_cast<int>(contents.size());
    for (int row = 0; row < contents_size; row++) // Read "rows"/lines of file.
    {
        std::vector<std::string> results = util::str_split(contents[row], ',');
        int results_size = static_cast<int>(results.size());
        for (int column = 0; column < results_size; column++) // Read "columns"/every item in line delimited by comma.
        {
            // Define constant data based on tilemap photo.
            // TODO: Add a way for these value to be either decided automatically, or tweaked within a config file.
            const int tile_size = 32;
            const int tilemap_cols = 10;
            const int tilemap_rows = 3;

            // Find the x and y index of the tile, relative to the tilemap image size.
            const int tile_index = atoi(results[column].c_str());
            int x_index = tile_index % tilemap_cols;
            int y_index = 0;
            for (int i = 0; i < tilemap_rows; i++)
            {
                int smallest_col = i * tilemap_cols;
                int biggest_col = smallest_col + (tilemap_cols - 1);
                if (!(tile_index < smallest_col) && !(tile_index > biggest_col))
                {
                    y_index = i;
                    break;
                }
            }

            // Create tile entity.
            // The tile x and y index are multipled by the tile size to offset the srcRect of jungle-tileset.
            ecs::entity tile = _registry->create_entity();
            tile.add_component<components::transform_component>(glm::vec2(column * tile_size, row * tile_size), glm::vec2(1.0f, 1.0f), 0.0);
            tile.add_component<components::sprite_component>("jungle-tileset", tile_size, tile_size, x_index * tile_size, y_index * tile_size);
        }
    }

    // Setup tank entity.
    ecs::entity tank = _registry->create_entity();
    tank.add_component<components::transform_component>(glm::vec2(10.0f, 10.0f), glm::vec2(1.0f, 1.0f), 0.0);
    tank.add_component<components::rigidbody_component>(glm::vec2(50.0f, 0.0f));
    tank.add_component<components::sprite_component>("tank-image", 32, 32);

    // Setup truck entity.
    ecs::entity truck = _registry->create_entity();
    truck.add_component<components::transform_component>(glm::vec2(10.0f, 50.0f), glm::vec2(1.0f, 1.0f), 90.0);
    truck.add_component<components::rigidbody_component>(glm::vec2(0.0f, 20.0f));
    truck.add_component<components::sprite_component>("truck-image", 32, 32);
}

void engine::game::setup()
{

}

void engine::game::process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EventType::SDL_QUIT:
                _is_running = false;
                break;

            case SDL_EventType::SDL_KEYDOWN:
                if (event.key.keysym.sym == SDL_KeyCode::SDLK_ESCAPE)
                {
                    _is_running = false;
                }
                break;
        }
    }
}

void engine::game::enforce_frame_rate()
{
    // If we are running too fast, waste some time until we reach miliseconds per frame.
    Uint32 time_to_wait = get_ms_per_frame() - (SDL_GetTicks() - _ms_prev_frame);
    if (time_to_wait > 0 && time_to_wait <= _ms_prev_frame)
    {
        SDL_Delay(time_to_wait);
    }

    // Calculate the number of seconds since the last frame.
    _delta_time = (SDL_GetTicks() - _ms_prev_frame) / 1000.0;

    // Store "previous" frame time.
    _ms_prev_frame = SDL_GetTicks();
}

void engine::game::update()
{
    // Update systems.
    _registry->get_system<systems::movement_system>().update(_delta_time);

    // Update registry to process pending entities.
    _registry->update();
}

void engine::game::render()
{
    // Draw background.
    SDL_SetRenderDrawColor(_renderer, 21, 21, 21, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    _registry->get_system<systems::render_system>().update(_renderer);

    // Swap back buffer with front buffer.
    SDL_RenderPresent(_renderer);
}

void engine::game::destroy()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
