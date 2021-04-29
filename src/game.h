#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#include <SDL2/SDL.h>
#include "ecs.h"

namespace engine
{
    class game
    {
        private:
            bool _is_running;
            Uint32 _ms_prev_frame;
            double _delta_time;

            SDL_Window* _window;
            SDL_Renderer* _renderer;

            std::unique_ptr<ecs::registry> _registry;

            int get_ms_per_frame();

        public:
            game();
            ~game();

            int target_fps = 60;
            int window_width;
            int window_height;

            void initialize();

            void run();
            void load_level(int number);
            void setup();
            void process_input();
            void enforce_frame_rate();
            void update();
            void render();

            void destroy();
    };
}

#endif
