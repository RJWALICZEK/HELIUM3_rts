#include "EntityManager.h"
#include <vector>

void EntityManager::init() {
    units.emplace_back(400.0f, 600.0f, UnitType::Worker);
    units.emplace_back(500.0f, 600.0f, UnitType::Worker);
    units.emplace_back(600.0f, 600.0f, UnitType::Soldier);

    buildings.emplace_back(500.0f, 700.0f, BuildingType::Base);
    buildings.emplace_back(700.0f, 700.0f, BuildingType::Barracks);
};

void EntityManager::update(float deltaTime) {
    for (auto& unit : units) {
        unit.update(deltaTime);
        //automatic soldier attack
        if (!unit.isSelected() && unit.getType() == UnitType::Soldier) {
            for (auto& building : buildings) {
                if (unit.isInRange(building)) {
                    unit.attack(building, deltaTime);
                    break;
                }
            }
        }
    }
    for (auto& building : buildings) {
        building.update(deltaTime);
        if (building.productionFinished() && building.isBarracks()) {
            float spawnX = building.getX() + building.getWidth() + 20.0f;
            float spawnY = building.getY() + building.getHeight() / 2.0f;
            units.emplace_back(spawnX, spawnY, UnitType::Soldier);
            printf(" New soldier spawned \n");
        }
    }
};
void EntityManager::render(SDL_Renderer* renderer, float camX, float camY) {
    //display buildings
    for (auto& building : buildings) {
        building.render(renderer, camX, camY);
    }
    for (auto& unit : units) {
        unit.render(renderer, camX, camY);
    }
};

void EntityManager::spawnUnit(float x, float y, UnitType type) {
    units.emplace_back(x, y, type);
};


std::vector<Unit>& EntityManager::getUnits() { return units; }
std::vector<Building>& EntityManager::getBuildings() { return buildings; }
