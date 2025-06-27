#include "Game.hpp"

Game::Game()
{
    window = new Window();    
}

Game::~Game()
{
   delete window; 
}

GameState Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return GameState::FAILURE;
    }

    if(window->CreateWindow() != GameState::SUCCESS)
    {
        window->~Window();
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
            return GameState::FAILURE;
        }
    }

    return GameState::SUCCESS;
}

GameState Game::Run()
{
    while(gameRunning)
    {
        if(Events() != GameState::SUCCESS)
        {
            return GameState::FAILURE;
        }


    }

    return GameState::SUCCESS;
}

void Game::clean()
{
    delete window;
    SDL_Quit();
}
