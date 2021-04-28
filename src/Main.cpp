#include "game/game.h"

int main(int argc, char* argv[])
{
    engine::Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
