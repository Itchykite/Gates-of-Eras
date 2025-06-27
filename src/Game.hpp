#pragma once

#include "Window/Window.hpp"
#include "Renderer/Renderer.hpp"
#include "Ext/Ext.hpp"
#include "Tilemap/Tilemap.hpp"

#include <string>

#include <SDL2/SDL.h>

class Game
{
public:
    Game();
    ~Game();

    bool LoadConfig(const std::string& filename);

    GameState Initialize();
    GameState Events();
    GameState Run();
    void clean();

private:
    int displayIndex = 0;
    bool fullscreen = false;
    int width = 800;
    int height = 600;

    bool gameRunning = true;

    Window* window;   
    Renderer* renderer;
    Tilemap* tilemap;
};

