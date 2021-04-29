#include "game.h"

int main(int argc, char* argv[])
{
    engine::game game;

    game.initialize();
    game.run();
    game.destroy();

    return 0;
}
