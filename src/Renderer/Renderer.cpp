#include "Renderer.hpp"
#include "../Window/Window.hpp"

Renderer::Renderer()
{
    renderer = nullptr;
}

Renderer::~Renderer()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

SDL_Renderer* Renderer::getRenderer()
{
    return renderer;
}

GameState Renderer::CreateRenderer(Window*& window)
{
    renderer = SDL_CreateRenderer(window->getWindow(), -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        return GameState::FAILURE;
    }

    return GameState::SUCCESS;
}

void Renderer::RenderClear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::RenderPresent()
{
    SDL_RenderPresent(renderer);
}

void Renderer::RenderRect()
{
    SDL_Rect rect = { 100, 100, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
