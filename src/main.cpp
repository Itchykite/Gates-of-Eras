#include "Game.hpp"

int main()
{
    Game* game = new Game;  
    if(game->Initialize() != GameState::SUCCESS)
    {
        delete game;
        return -1;
    }
    game->Run();

    delete game;

    return 0;
}
