#pragma once
#include "Game.h"
#include <SDL2/SDL.h>

enum class BuildingType {
    Base,
    Barracks
};

class Building {
private:
    float posX;
    float posY;
    float width;
    float height;
    float productionTime = 0.0f;
    float productionTimer = 0.0f;

    bool isProducing = false;

    BuildingType type;
public:
    Building(float x, float y, BuildingType type);
    ~Building() = default;

    void render(SDL_Renderer* renderer);
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    void startProduction();
    void update(float deltaTime);
    bool isBarracks() const;
    bool productionFinished();
};