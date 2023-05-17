#include "game.hpp"
int main(void)
{
    Game* game = new Game();
    game->Start();
    delete game;
    return 0;
}