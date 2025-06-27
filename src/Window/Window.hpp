#pragma once

#include <SDL2/SDL.h>
#include "../Ext/Ext.hpp"

class Window
{
public:
    Window();
    ~Window();

    GameState CreateWindow();

private:
    SDL_Window* window;

    int WINDOW_WIDTH = 1920;
    int WINDOW_HEIGHT = 1080;
};
