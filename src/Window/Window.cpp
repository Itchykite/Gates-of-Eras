#include "Window.hpp"
#include <SDL2/SDL_video.h>

Window::Window() : window(nullptr) {}

GameState Window::CreateWindow(int displayIndex, int width, int height, Uint32 flags)
{
    SDL_Rect displayBounds;
    if (SDL_GetDisplayBounds(displayIndex, &displayBounds) != 0)
    {
        SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
        return GameState::FAILURE;
    }
    
    int x = displayBounds.x + (displayBounds.w - width) / 2;
    int y = displayBounds.y + (displayBounds.h - height) / 2;
    
    window = SDL_CreateWindow("Gates of Eras", x, y, width, height, flags);
    if(!window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return GameState::FAILURE;
    }
    
    return GameState::SUCCESS;
}

SDL_Window* Window::getWindow()
{
    return window;
}

Window::~Window()
{
    if(window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}
