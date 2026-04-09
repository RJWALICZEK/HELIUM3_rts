#pragma once
#include "Game.h"
#include <SDL2/SDL.h>

enum class UnitType {
    Worker,
    Soldier
};

class Unit {
private:
    float posX;
    float posY;

    float targetX = 0.0f;
    float targetY = 0.0f;
    float speed = 100.0f;

    bool isMoving = false;
    bool selected = false;

    UnitType type;
public:
    Unit(float x, float y, UnitType type);
    ~Unit() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    float getX() const { return posX; };
    float getY() const { return posY; };

    void select();
    void deselect();
    bool isSelected() const;

    void moveTo(float targetX, float targetY);
    void setSpeed(float newSpeed);

};