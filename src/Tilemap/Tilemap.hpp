#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Tilemap 
{
public:
    Tilemap();
    ~Tilemap();


    bool loadTileset(SDL_Renderer* renderer, const std::string& path, int tileSize);
    bool loadMapFromFile(const std::string& path);
    bool createEmptyMap(int width, int height, int defaultTile = 0);
    
    int getTile(int x, int y) const;
    void setTile(int x, int y, int tileId);
    
    void setScale(float scale);
    void moveCamera(int x, int y);
    
    void render(SDL_Renderer* renderer, int windowWidth, int windowHeight);
    
    bool saveMapToFile(const std::string& path);
    
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    int getTileSize() const { return tileSize; }

private:
    SDL_Texture* tilesetTexture;
    std::vector<std::vector<int>> mapData;

    int mapWidth;
    int mapHeight;   
    int tileSize;     
    int tilesPerRow; 
    
    float cameraScale;
    int cameraX;
    int cameraY;
};
