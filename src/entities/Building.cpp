#include "Building.h"
#include "Game.h"
#include <cstdio>

Building::Building(float x, float y, BuildingType type, Team team)
    : posX(x), posY(y), width(96.0f), height(96.0f), type(type), team(team), hp(0), maxHP(0)
{
    switch (type) {
    case BuildingType::Barracks:
    {
        maxHP = 3200;
        hp = maxHP;
        break;
    }

    case BuildingType::Base:
    {
        maxHP = 7200;
        hp = maxHP;
        break;
    }

    default: { return; }

           hp = 1000;

    }

    printf(" created building type %d at pos: %.1f | %.1f\n",
        static_cast<int>(type), x, y);
}

void Building::render(SDL_Renderer* renderer, float camX, float camY) {
    if (!renderer) {
        return;
    }

    int screenX = static_cast<int>(posX - camX);
    int screenY = static_cast<int>(posY - camY);

    if (type == BuildingType::Base) {
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    }
    if (type == BuildingType::Barracks) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }

    SDL_Rect rect = { screenX, screenY, static_cast<int>(width), static_cast<int>(height) };
    SDL_RenderFillRect(renderer, &rect);
    renderHPBar(renderer, screenX, screenY);
    renderProductionBar(renderer, screenX, screenY);
}
void Building::renderProductionBar(SDL_Renderer* renderer, int screenX, int screenY) {
    if (!isProducing || productionTime <= 0.0f) { return; }
    float progress = productionTimer / productionTime;
    if (progress > 1.0f) { progress = 1.0f; }
    if (progress < 0.0f) { progress = 0.0f; }

    SDL_SetRenderDrawColor(renderer, 0, 255, 50, 255);

    SDL_Rect progressBar = {
        screenX,
        screenY - 12,
        static_cast<int>(width * progress),
        8
    };
    SDL_RenderFillRect(renderer, &progressBar);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &progressBar);

}
void Building::renderHPBar(SDL_Renderer* renderer, int screenX, int screenY) {
    if (maxHP <= 0) { return; }
    if (hp < 0) { hp = 0; }
    float percent = static_cast<float>(hp) / static_cast<float>(maxHP);
    if (percent > 1.0f) { percent = 1.0f; }
    if (percent < 0) { percent = 0.0f; }

    int barX = screenX;
    int barY = (screenY + static_cast<int>(height)) + 6;
    int barWidth = static_cast<int>(width * percent);

    //background
    SDL_Rect bg = {
        barX,
        barY,
        static_cast<int>(width),
        6
    };
    SDL_SetRenderDrawColor(renderer, 60, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg);
    //bar
    SDL_Rect HPBar = {
        barX,
        barY,
        barWidth,
        6
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &HPBar);
    //farame
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bg);


}

void Building::startProduction() {
    if (type == BuildingType::Barracks && !isProducing) {

        isProducing = true;
        productionTimer = 0.0f;
        productionTime = 7.0f;
        printf("Started producing soldier...\n");
    }
    else if (type == BuildingType::Base && !isProducing) {

        isProducing = true;
        productionTimer = 0.0f;
        productionTime = 12.0f;
        printf("Started producing worker...\n");
    }
}

void Building::update(float deltaTime) {
    if (isProducing && type == BuildingType::Barracks) {
        productionTimer += deltaTime;
    }
    if (isProducing && type == BuildingType::Base) {
        productionTimer += deltaTime;
    }

}

bool Building::isBarracks() const {
    return type == BuildingType::Barracks;
}
bool Building::isBase() const {
    return type == BuildingType::Base;
}

bool Building::productionFinished() {
    if (type == BuildingType::Barracks && isProducing && productionTimer >= productionTime) {
        isProducing = false;
        productionTimer = 0.0f;
        return true;
    }
    if (type == BuildingType::Base && isProducing && productionTimer >= productionTime) {
        isProducing = false;
        productionTimer = 0.0f;
        return true;
    }
    return false;
}

void Building::takeDamage(int damage) {
    if (hp > 0) {
        hp -= damage;
        printf("Building hit! hp left: %d \n", hp);
    }
}

bool Building::isDestroyed() const {

    return hp <= 0;
}
