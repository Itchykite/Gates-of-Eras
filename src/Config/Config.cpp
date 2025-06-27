#include "Config.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct DisplayOption 
{
    int index;
    std::string name;
    SDL_Rect bounds;
};

struct GameConfig 
{
    int displayIndex = 0;
    bool fullscreen = true;
    int width = 1280;
    int height = 720;
};

struct Button
{
    SDL_Rect rect;
    std::string text;
    bool selected = false;
};

SDL_Window* configWindow = nullptr;
SDL_Renderer* configRenderer = nullptr;
TTF_Font* font = nullptr;
TTF_Font* ruritania = nullptr;

int configWindowWidth = 600;
int configWindowHeight = 600;

std::vector<DisplayOption> displays;
std::vector<Button> buttons;
GameConfig config;

bool isRunning = true;
bool configSaved = false;

void SaveConfig() 
{
    std::ofstream file("game_config.ini");
    if (file.is_open()) 
    {
        file << "DisplayIndex=" << config.displayIndex << std::endl;
        file << "FullScreen=" << (config.fullscreen ? "true" : "false") << std::endl;
        file << "Width=" << config.width << std::endl;
        file << "Height=" << config.height << std::endl;
        file.close();
    }
}

void LoadConfig()
{
    std::ifstream file("game_config.ini");
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
                
                if (key == "DisplayIndex") config.displayIndex = std::stoi(value);
                else if (key == "FullScreen") config.fullscreen = (value == "true");
                else if (key == "Width") config.width = std::stoi(value);
                else if (key == "Height") config.height = std::stoi(value);
            }
        }
        file.close();
    }
}

void RenderText(const char* text, int x, int y, SDL_Color color) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(configRenderer, surface);
    
    int width = surface->w;
    int height = surface->h;
    
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(configRenderer, texture, NULL, &destRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderText(const char* text, int x, int y, SDL_Color color, TTF_Font* font) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(configRenderer, surface);
    
    int width = surface->w;
    int height = surface->h;
    
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(configRenderer, texture, NULL, &destRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void InitUI() 
{
    buttons.push_back({SDL_Rect{300, 500, 200, 50}, "Start Game", false});
    buttons.push_back({SDL_Rect{50, 500, 200, 50}, "Exit", false});
    
    if (TTF_Init() == -1) 
    {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
        return;
    }
    
    font = TTF_OpenFont("../assets/fonts/roboto-regular.ttf", 18);
    if (!font)
    {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
    }

    ruritania = TTF_OpenFont("../assets/fonts/ruritania.ttf", 42);
    if (!ruritania)
    {
        SDL_Log("Failed to load Ruritania font! SDL_ttf Error: %s", TTF_GetError());
    }
}

void DetectDisplays()
{
    displays.clear();
    
    int numDisplays = SDL_GetNumVideoDisplays();
    for (int i = 0; i < numDisplays; ++i) 
    {
        DisplayOption display;
        display.index = i;
        display.name = SDL_GetDisplayName(i);
        
        SDL_GetDisplayBounds(i, &display.bounds);
        
        displays.push_back(display);
    }
    
    if (config.displayIndex >= displays.size())
    {
        config.displayIndex = 0;
    }
}

void HandleEvents()
{
    SDL_Event event;
    int mouseX, mouseY;
    
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
                
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                
                for (auto& button : buttons)
                {
                    if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
                        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) {
                        button.selected = true;
                    } 
                    else 
                    {
                        button.selected = false;
                    }
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                mouseX = event.button.x;
                mouseY = event.button.y;
                
                if (buttons[0].selected) 
                { 
                    SaveConfig();
                    configSaved = true;
                    isRunning = false;
                }
                else if (buttons[1].selected)
                {
                    isRunning = false;
                }
                
                for (size_t i = 0; i < displays.size(); ++i)
                {
                    SDL_Rect selectRect = {50, 100 + (int)i * 30, 20, 20};
                    if (mouseX >= selectRect.x && mouseX <= selectRect.x + selectRect.w &&
                        mouseY >= selectRect.y && mouseY <= selectRect.y + selectRect.h)
                    {
                        config.displayIndex = (int)i;
                    }
                }
                
                SDL_Rect fullscreenRect = {50, 350, 20, 20};
                if (mouseX >= fullscreenRect.x && mouseX <= fullscreenRect.x + fullscreenRect.w &&
                    mouseY >= fullscreenRect.y && mouseY <= fullscreenRect.y + fullscreenRect.h) 
                {
                    config.fullscreen = !config.fullscreen;
                }
                
                break;
        }
    }
}

void RenderUI() 
{
    SDL_SetRenderDrawColor(configRenderer, 50, 50, 50, 255);
    SDL_RenderClear(configRenderer);
    
    SDL_Color textColor = {255, 255, 255, 255};
    
    RenderText("Game Configuration", configWindowWidth / 4 - 25 , 5, {255, 0, 0, 255}, ruritania);
    
    RenderText("Select Display:", 50, 70, textColor);
    for (size_t i = 0; i < displays.size(); ++i) 
    {
        SDL_Rect selectRect = {50, 100 + (int)i * 30, 20, 20};
        
        if (config.displayIndex == (int)i)
        {
            SDL_SetRenderDrawColor(configRenderer, 0, 255, 0, 255);
            SDL_RenderFillRect(configRenderer, &selectRect);
        }
        
        SDL_SetRenderDrawColor(configRenderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(configRenderer, &selectRect);
        
        char displayInfo[256];
        sprintf(displayInfo, "%s (%dx%d)", displays[i].name.c_str(), 
                displays[i].bounds.w, displays[i].bounds.h);
        RenderText(displayInfo, 80, 100 + (int)i * 30, textColor);
    }
    
    SDL_Rect fullscreenRect = {50, 350, 20, 20};
    if (config.fullscreen)
    {
        SDL_SetRenderDrawColor(configRenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(configRenderer, &fullscreenRect);
    }

    SDL_SetRenderDrawColor(configRenderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(configRenderer, &fullscreenRect);
    RenderText("Fullscreen", 80, 350, textColor);
    
    for (const auto& button : buttons) 
    {
        if (button.selected) 
        {
            SDL_SetRenderDrawColor(configRenderer, 100, 100, 200, 255);
        } 
        else 
        {
            SDL_SetRenderDrawColor(configRenderer, 70, 70, 150, 255);
        }
        
        SDL_RenderFillRect(configRenderer, &button.rect);
        
        SDL_SetRenderDrawColor(configRenderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(configRenderer, &button.rect);
        
        int textWidth = (int)button.text.length() * 10; 
        int textX = button.rect.x + (button.rect.w - textWidth) / 2;
        int textY = button.rect.y + (button.rect.h - 18) / 2;
        
        RenderText(button.text.c_str(), textX, textY, textColor);
    }
    
    SDL_RenderPresent(configRenderer);
}

bool RunConfigWindow() 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }
    
    LoadConfig();
    
    DetectDisplays();
    
    configWindow = SDL_CreateWindow("Game Configuration", 
                                   SDL_WINDOWPOS_CENTERED, 
                                   SDL_WINDOWPOS_CENTERED,
                                   configWindowWidth, configWindowHeight, 
                                   SDL_WINDOW_SHOWN);
                                   
    if (!configWindow) 
    {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }
    
    configRenderer = SDL_CreateRenderer(configWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!configRenderer) 
    {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(configWindow);
        SDL_Quit();
        return false;
    }
    
    InitUI();
    
    while (isRunning) 
    {
        HandleEvents();
        RenderUI();
        SDL_Delay(16);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(configRenderer);
    SDL_DestroyWindow(configWindow);
    
    return configSaved;
}
