#pragma once
#include "Game.h"
#include <SDL2/SDL.h>

enum class BuildingType {
    Base
};

class Building {
private:
    float posX;
    float posY;
    float width;
    float height;
    BuildingType type;
public:
    Building(float x, float y, BuildingType type);
    ~Building() = default;

    void render(SDL_Renderer* renderer);
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getWidtth() const { return width; }
    float getHeight() const { return height; }
};