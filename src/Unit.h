#pragma once
#include "Game.h"
#include <SDL2/SDL.h>

enum class UnitType {
    Worker
};

class Unit {
private:
    float posX;
    float posY;
    UnitType type;
public:
    Unit(float x, float y, UnitType type);
    ~Unit() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    float GetX() const { return posX; };
    float GetY() const { return posY; };

};