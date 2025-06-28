#include "Tilemap.hpp"
#include <algorithm>

Tilemap::Tilemap()
    : tilesetTexture(nullptr), mapWidth(0), mapHeight(0), tileSize(32), tilesPerRow(0),
      cameraScale(1.0f), cameraX(0.0f), cameraY(0.0f) {}

Tilemap::~Tilemap() 
{
    if (tilesetTexture) 
    {
        SDL_DestroyTexture(tilesetTexture);
    }
}

bool Tilemap::loadTileset(SDL_Renderer* renderer, const std::string& path, int tileSize) 
{
    if (tilesetTexture != nullptr) 
    {
        SDL_DestroyTexture(tilesetTexture);
        tilesetTexture = nullptr;
    }
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    
    tilesetTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!tilesetTexture) 
    {
        SDL_Log("Failed to load tileset: %s", SDL_GetError());
        return false;
    }
    
    int textureWidth;
    SDL_QueryTexture(tilesetTexture, nullptr, nullptr, &textureWidth, nullptr);
    
    this->tileSize = tileSize;
    this->tilesPerRow = textureWidth / tileSize;
    
    return true;
}

bool Tilemap::loadMapFromFile(const std::string& path) 
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        SDL_Log("Failed to open map file: %s", path.c_str());
        return false;
    }
    
    file >> mapWidth >> mapHeight;
    
    mapData.resize(mapHeight, std::vector<int>(mapWidth, 0));
    
    for (int y = 0; y < mapHeight; ++y) 
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            file >> mapData[y][x];
        }
    }
    
    file.close();
    return true;
}

bool Tilemap::createEmptyMap(int width, int height, int defaultTile) 
{
    mapWidth = width;
    mapHeight = height;
    
    mapData.resize(mapHeight, std::vector<int>(mapWidth, defaultTile));
    
    return true;
}

int Tilemap::getTile(int x, int y) const 
{
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) 
    {
        return mapData[y][x];
    }

    return -1; 
}

void Tilemap::setTile(int x, int y, int tileId) 
{
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) 
    {
        mapData[y][x] = tileId;
    }
}

void Tilemap::setScale(float scale) 
{
    cameraScale = (scale > 0.1f) ? scale : 0.1f;
}

void Tilemap::moveCamera(int x, int y)
{
    cameraX = x;
    cameraY = y;
}

void Tilemap::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    if (!tilesetTexture || mapWidth == 0 || mapHeight == 0)
    {
        return;
    }
    
    int scaledTileSize = static_cast<int>(tileSize * cameraScale);
    
    int startX = std::max(0, cameraX / scaledTileSize);
    int startY = std::max(0, cameraY / scaledTileSize);
    int endX = std::min(mapWidth, (cameraX + windowWidth) / scaledTileSize + 1);
    int endY = std::min(mapHeight, (cameraY + windowHeight) / scaledTileSize + 1);

    for (int y = startY; y < endY; ++y) 
    {
        for (int x = startX; x < endX; ++x)
        {
            int tileId = mapData[y][x];
            
            if (tileId == -1) continue;
            
            SDL_Rect srcRect = 
            {
                (tileId % tilesPerRow) * tileSize,
                (tileId / tilesPerRow) * tileSize,
                tileSize,
                tileSize
            };
            
            SDL_Rect destRect = 
            {
                static_cast<int>(x * scaledTileSize - cameraX),
                static_cast<int>(y * scaledTileSize - cameraY),
                scaledTileSize,
                scaledTileSize
            };
            
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
        }
    }
}

bool Tilemap::saveMapToFile(const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        SDL_Log("Failed to create map file: %s", path.c_str());
        return false;
    }
    
    file << mapWidth << " " << mapHeight << std::endl;
    
    for (int y = 0; y < mapHeight; ++y) 
    {
        for (int x = 0; x < mapWidth; ++x) 
        {
            file << mapData[y][x] << " ";
        }

        file << std::endl;
    }
    
    file.close();
    return true;
}
