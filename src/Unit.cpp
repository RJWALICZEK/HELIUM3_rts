#include "Unit.h"
#include <cstdio>

Unit::Unit(float x, float y, UnitType type)
    : posX(x), posY(y), type(type)
{
    printf("Created unit, type: %d , pos: %1.f | %1.f \n",
        static_cast<int>(type), x, y);
}

void Unit::update(float deltaTime) {
    //movement, colect resorcesetc
    (void)deltaTime;
}

void Unit::render(SDL_Renderer* renderer) {

    if (!renderer) {
        return;
    }

    //blue rectangle - unit representation 28x28px
    if (selected) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
    }

    SDL_Rect rect = {
        static_cast<int>(posX),
        static_cast<int>(posY),
        28,
        28
    };

    SDL_RenderFillRect(renderer, &rect);
}

void Unit::select() {
    selected = true;
    printf("Unit selected \n");
}

void Unit::deselect() {
    selected = false;
}

bool Unit::isSelected() const {
    return selected;
}