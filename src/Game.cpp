#include "Game.hpp"

Game::Game()
{
    window = new Window();    
    renderer = new Renderer();
}

Game::~Game() 
{
    clean();
}

GameState Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return GameState::FAILURE;
    }

    if(window->CreateWindow() != GameState::SUCCESS)
    {
        window->~Window();
        return GameState::FAILURE;
    }

    if(renderer->CreateRenderer(window) != GameState::SUCCESS)
    {
        renderer->~Renderer();
        return GameState::FAILURE;
    }

    return GameState::SUCCESS; 
}

GameState Game::Events()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            gameRunning = false;
        }
    }

    return GameState::SUCCESS;
}

GameState Game::Run()
{
    while(gameRunning)
    {
        if(Events() == GameState::FAILURE)
        {
            return GameState::FAILURE;
        }

        renderer->RenderClear();
        renderer->RenderRect();
        renderer->RenderPresent();
    }

    return GameState::SUCCESS;
}

void Game::clean()
{
    delete window;
    delete renderer;
    SDL_Quit();
}
