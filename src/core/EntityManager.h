#pragma once
#include <vector>
#include "Building.h"
#include "Unit.h"

class EntityManager {
private:
    std::vector<Unit> units;
    std::vector<Building> buildings;

public:
    void init(); //start spawn

    void update(class World& world, class Player& player, float deltaTime);
    void render(SDL_Renderer* renderer, float camX, float camY);

    std::vector<Unit>& getUnits() { return units; }
    std::vector<Building>& getBuildings() { return buildings; }

    void spawnUnit(float x, float y, UnitType type);
};

//from game.h
/*  std::vector<Unit> units;
    std::vector<Building> buildings;*/