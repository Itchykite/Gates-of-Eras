#include "Game.hpp"

#include <fstream>

Game::Game()
{
    window = new Window();    
    renderer = new Renderer();
    tilemap = new Tilemap();
}

Game::~Game() 
{
    clean();
}

bool Game::LoadConfig(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.is_open()) 
    {
        std::string line;
        while (std::getline(file, line)) 
        {
            size_t pos = line.find('=');
            if (pos != std::string::npos) 
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                
                if (key == "DisplayIndex") displayIndex = std::stoi(value);
                else if (key == "FullScreen") fullscreen = (value == "true");
                else if (key == "Width") width = std::stoi(value);
                else if (key == "Height") height = std::stoi(value);
            }
        }
        file.close();
   
        return true;
    }

    return false;
}

GameState Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return GameState::FAILURE;
    }

    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    if (fullscreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    if(window->CreateWindow(displayIndex, width, height, windowFlags) != GameState::SUCCESS)
    {
        window->~Window();
        return GameState::FAILURE;
    }

    if(renderer->CreateRenderer(window) != GameState::SUCCESS)
    {
        renderer->~Renderer();
        return GameState::FAILURE;
    }

    if(!(tilemap->loadTileset(renderer->getRenderer(), "../assets/map/tileset.png", 32)))
    {
        SDL_Log("Failed to load tileset");
        return GameState::FAILURE;
    }

    if(!(tilemap->loadMapFromFile("../assets/map/map.txt")))
    {
        SDL_Log("Failed to load map from file");
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

        tilemap->render(renderer->getRenderer(), window->getWidth(), window->getHeight());

        renderer->RenderPresent();
    }

    return GameState::SUCCESS;
}

void Game::clean()
{
    delete window;
    delete renderer;
    delete tilemap;
    SDL_Quit();
}
