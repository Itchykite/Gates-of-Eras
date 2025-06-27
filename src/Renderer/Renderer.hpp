#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "../Ext/Ext.hpp"

class Window;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    GameState CreateRenderer(Window*& window);
    void Clear();

    void RenderClear();
    void RenderPresent();
    void RenderRect();

private:
    SDL_Renderer* renderer;
};
