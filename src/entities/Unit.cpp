#include "Unit.h"
#include "World.h"
#include "Building.h"
#include <cstdio>
#include <cmath>
#include <vector>

Unit::Unit(float x, float y, UnitType type, Team team)
    : posX(x), posY(y), type(type), team(team),
    collectTimer(0.0f), isCollecting(false)
{
    printf(" Created unit, type: %d , pos: %1.f | %.1f \n",
        static_cast<int>(type), x, y);
}

void Unit::update(World& world, float deltaTime) {
    if (state != UnitState::Moving) {
        return;
    }
    //movement, colect resorcesetc
    //unit direction
    float dx = targetX - posX;
    float dy = targetY - posY;
    float distance = std::sqrt(dx * dx + dy * dy);
    float stopDistance = 5.0f;

    if (distance < stopDistance) {
        posX = targetX;
        posY = targetY;
        state = UnitState::Idle;
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

void Unit::render(SDL_Renderer* renderer, float camX, float camY) {
    if (!renderer) {
        return;
    }

    int screenX = static_cast<int>(posX - camX);
    int screenY = static_cast<int>(posY - camY);

    if (selected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    }
    else if (type == UnitType::Worker) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    }
    else if (type == UnitType::Soldier) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    SDL_Rect rect = {
        screenX,
        screenY,
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
    attackTarget = nullptr;
    state = UnitState::Moving;
    printf(" Unit move to %.1f | %1.f \n", tx, ty);
}

void Unit::setSpeed(float newSpeed) {
    if (newSpeed > 0.0f) {
        speed = newSpeed;
    }
}
void Unit::findTarget(std::vector<Building>& buildings) {
    if (type != UnitType::Soldier) {
        return;
    }
    if (state != UnitState::Idle) {
        return;
    }

    attackTarget = nullptr;
    for (auto& building : buildings) {
        if (building.getTeam() == team) {
            continue;
        }
        if (isInRange(building)) {
            attackTarget = &building;
            state = UnitState::Attacking;
            return;
        }
    }
}
void Unit::updateAttack(float deltaTime) {
    if (!attackTarget || !isInRange(*attackTarget)) {
        attackTarget = nullptr;
        if (state == UnitState::Attacking) {
            state = UnitState::Idle;
        }
        return;
    }
    if (state == UnitState::Moving) {
        attackTarget = nullptr;
        return;
    }
    if (state != UnitState::Attacking) {
        return;
    }

    attackTimer += deltaTime;

    if (attackTimer >= attackCooldown) {
        attackTarget->takeDamage(attackDmg);
        attackTimer = 0.0f;
        printf(" Soldier deal %d dmg", attackDmg);
    }
    if (attackTarget->isDestroyed()) {
        attackTarget = nullptr;
        state = UnitState::Idle;
    }
}

bool Unit::isInRange(const Building& target) const {
    // range of attack , 100 px
    float dx = target.getX() + target.getWidth() / 2.0f - posX;
    float dy = target.getY() + target.getHeight() / 2.0f - posY;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= 120.0f;
}

int Unit::updateCollecting(World& world, float deltaTime) {
    if (!isCollecting || !isWorker() || state == UnitState::Moving) {
        return 0;
    }

    collectTimer += deltaTime;

    //cellect 1 resource / sec
    if (collectTimer >= 1.0f) {
        float nodeX = 0.0f;
        float nodeY = 0.0f;
        if (world.isNearResource(posX, posY, nodeX, nodeY)) {
            int collected = world.takeResource(nodeX, nodeY, 7);
            if (collected > 0) {
                printf("worcer collected %d helium3\n", collected);
            }

            collectTimer = 0.0f;
            return collected;

        }
        else {
            isCollecting = false;
            collectTimer = 0.0f;
        }
        //********check distance from nearest resource**********
    }
    return 0;
}
bool Unit::isWorker() const {
    return type == UnitType::Worker;
}
void Unit::tryStartCollecting(World& world) {
    if (!isWorker() || isCollecting) {
        return;
    }
    float nodeX = 0.0f;
    float nodeY = 0.0f;
    if (world.isNearResource(posX, posY, nodeX, nodeY)) {
        isCollecting = true;
        collectTimer = 0.0f;
        //moveTo(nodeX + 8.0f, nodeY + 8.0f);
    }
}
