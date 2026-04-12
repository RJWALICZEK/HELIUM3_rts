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

    void update(float deltaTime);
    void render(SDL_Renderer* renderer, float camX, float camY);

    std::vector<Unit>& getUnits();
    std::vector<Building>& getBuildings();

    void spawnUnit(float x, float y, UnitType type);
};

//from game.h
/*  std::vector<Unit> units;
    std::vector<Building> buildings;*/