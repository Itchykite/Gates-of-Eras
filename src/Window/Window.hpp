#pragma once

#include <SDL2/SDL.h>
#include "../Ext/Ext.hpp"

class Window
{
public:
    Window();
    ~Window();

    GameState CreateWindow(int displayIndex, int width, int height, Uint32 flags);

    SDL_Window* getWindow();
    int getWidth() const;
    int getHeight() const;

private:
    SDL_Window* window;

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
};
