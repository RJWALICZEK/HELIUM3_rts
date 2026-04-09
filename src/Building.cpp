#include "Building.h"
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

    SDL_Rect rect = {
        static_cast<int>(posX),
        static_cast<int>(posY),
        static_cast<int>(width),
        static_cast<int>(height)
    };

    SDL_RenderFillRect(renderer, &rect);
}