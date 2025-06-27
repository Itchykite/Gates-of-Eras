#include "Game.hpp"
#include "Config/Config.hpp"

int main()
{
    bool config = RunConfigWindow();
    if(config)
    {
        Game* game = new Game;  

        game->LoadConfig("game_config.ini");

        if(game->Initialize() != GameState::SUCCESS)
        {
            delete game;
            return -1;
        }
        game->Run();

        delete game;
    }

    return 0;
}
