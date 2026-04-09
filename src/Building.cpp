#include "Building.h"
#include "Game.h"
#include <cstdio>

Building::Building(float x, float y, BuildingType type)
    : posX(x), posY(y), width(96.0f), height(96.0f), type(type) {
    printf(" created building type %d at pos: %.1f | %.1f\n",
        static_cast<int>(type), x, y);
}

void Building::render(SDL_Renderer* renderer) {

    if (!renderer) {
        return;
    }

    if (type == BuildingType::Base) {
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // grey: Base
    }
    else if (type == BuildingType::Barracks) {
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255); //dark green: Barracks
    }

    if (isProducing && type == BuildingType::Barracks) {
        float progress = productionTimer / productionTime;  //production in Barracks progress bar 
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);

        SDL_Rect progressBar = {
            static_cast<int>(posX),
            static_cast<int>(posY - 10),
            static_cast<int>(width * progress),
            6
        };
        SDL_RenderFillRect(renderer, &progressBar);
    }

    SDL_Rect rect = {
        static_cast<int>(posX),
        static_cast<int>(posY),
        static_cast<int>(width),
        static_cast<int>(height)
    };

    SDL_RenderFillRect(renderer, &rect);
}

void Building::startProduction() {
    if (type == BuildingType::Barracks && !isProducing) {
        isProducing = true;
        productionTime = 5.0f; //timer reset
        printf("Creating soldier unit in barracks...\n");
    }
}

void Building::update(float deltaTime) {

    if (!isProducing) {
        return;
    }

    productionTimer += deltaTime;

    if (productionTimer >= productionTime) {
        //creating soldier end, new unit will apear on map
        isProducing = false;
        productionTimer = 0.0f;
        printf(" debug , building update \n");
    }
}

bool Building::isBarracks() const {
    return type == BuildingType::Barracks;
}

bool Building::productionFinished() {
    if (isProducing && productionTime >= productionTimer) { // DO POPRAWY , KIEDY CZAS PRODUKCJI OSIAGNIE MAX MA BYC TRUE
        isProducing = false;
        productionTimer = 0.0f;
        return true;
    }
    return false;
}