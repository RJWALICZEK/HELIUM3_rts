#pragma once
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

    int hp;
    int maxHP;

    bool isProducing = false;


    BuildingType type;
    void renderProductionBar(SDL_Renderer* renderer, int screenX, int screenY);
    void renderHPBar(SDL_Renderer* renderer, int screenX, int screenY);
public:
    Building(float x, float y, BuildingType type);
    ~Building() = default;

    //old 
    //void render(SDL_Renderer* renderer);
    //new 
    void render(SDL_Renderer* renderer, float camX, float camY);

    float getX() const { return posX; }
    float getY() const { return posY; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool getProductingStatus() const { return isProducing; }
    int getHP() const { return hp; }

    void startProduction();
    void update(float deltaTime);
    bool isBarracks() const;
    bool isBase() const;
    bool productionFinished();
    void takeDamage(int damage);
    bool isDestroyed() const;
};