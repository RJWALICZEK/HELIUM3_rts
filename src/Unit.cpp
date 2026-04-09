#include "Unit.h"
#include <cstdio>
#include <cmath>

Unit::Unit(float x, float y, UnitType type)
    : posX(x), posY(y), type(type)
{
    printf(" Created unit, type: %d , pos: %1.f | %.1f \n",
        static_cast<int>(type), x, y);
}

void Unit::update(float deltaTime) {
    //movement, colect resorcesetc
    if (!isMoving) {
        return;
    }

    //unit direction
    float dx = targetX - posX;
    float dy = targetY - posY;
    float distance = sqrt(dx * dx + dy * dy);
    float stopDistance = 5.0f;

    if (distance < stopDistance) {
        posX = targetX;
        posY = targetY;
        isMoving = false;
        printf(" Unit reached destination \n");
        return;
    }
    if (distance > 0) {
        dx /= distance;
        dy /= distance;
    }

    posX += dx * speed * deltaTime;
    posY += dy * speed * deltaTime;
}

void Unit::render(SDL_Renderer* renderer) {

    if (!renderer) {
        return;
    }

    //blue rectangle - unit representation 28x28px
    if (selected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); //yellow selected
    }
    else if (type == UnitType::Worker) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);  //blue unselected worker
    }
    else if (type == UnitType::Soldier) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  //red unselected soldier
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
    printf(" Unit selected \n");
}

void Unit::deselect() {
    selected = false;
}

bool Unit::isSelected() const {
    return selected;
}

void Unit::moveTo(float tx, float ty) {
    targetX = tx;
    targetY = ty;
    isMoving = true;
    printf(" Unit move to %.1f | %1.f \n", tx, ty);
}

void Unit::setSpeed(float newSpeed) {
    if (newSpeed > 0.0f) {
        speed = newSpeed;
    }
}
