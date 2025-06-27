#include "Window.hpp"

Window::Window() : window(nullptr) {}

GameState Window::CreateWindow()
{
    window = SDL_CreateWindow("Gates of Eras", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
