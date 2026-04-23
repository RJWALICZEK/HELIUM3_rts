#pragma once
#include "GameTypes.h"
#include <SDL2/SDL.h>


class Building {
private:
    BuildingType type;
    Team team;
    float posX;
    float posY;
    float width;
    float height;
    float productionTime = 0.0f;
    float productionTimer = 0.0f;

    int hp;
    int maxHP;

    bool isProducing = false;

    void renderProductionBar(SDL_Renderer* renderer, int screenX, int screenY);
    void renderHPBar(SDL_Renderer* renderer, int screenX, int screenY);
public:
    Building(float x, float y, BuildingType type, Team team);
    ~Building() = default;

    void render(SDL_Renderer* renderer, float camX, float camY);

    float getX() const { return posX; }
    float getY() const { return posY; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool getProductingStatus() const { return isProducing; }
    int getHP() const { return hp; }
    Team getTeam() const { return team; }

    void startProduction();
    void takeDamage(int damage);
    bool productionFinished();
    void update(float deltaTime);

    bool isBarracks() const;
    bool isBase() const;
    bool isDestroyed() const;
};