#pragma once

#include "Window/Window.hpp"
#include "Ext/Ext.hpp"

class Game
{
public:
    Game();
    ~Game();

    GameState Initialize();
    GameState Events();
    GameState Run();
    void clean();

private:
    bool gameRunning = true;

    Window* window;   
};

