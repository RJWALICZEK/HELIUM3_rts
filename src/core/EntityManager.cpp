#include "EntityManager.h"
#include "World.h"
#include "Player.h"
#include <vector>

void EntityManager::init() {
    spawnUnit(400.0f, 600.0f, UnitType::Worker, Team::Player);
    spawnUnit(500.0f, 600.0f, UnitType::Worker, Team::Player);
    spawnUnit(600.0f, 600.0f, UnitType::Soldier, Team::Player);

    spawnBuilding(500.0f, 700.0f, BuildingType::Base, Team::Player);
    spawnBuilding(700.0f, 700.0f, BuildingType::Barracks, Team::Player);

    spawnUnit(1400.0f, 600.0f, UnitType::Worker, Team::Enemy);
    spawnUnit(1500.0f, 600.0f, UnitType::Worker, Team::Enemy);
    spawnUnit(1600.0f, 600.0f, UnitType::Soldier, Team::Enemy);

    spawnBuilding(1500.0f, 700.0f, BuildingType::Base, Team::Enemy);
    spawnBuilding(1700.0f, 700.0f, BuildingType::Barracks, Team::Enemy);
};

void EntityManager::update(World& world, Player& player, float deltaTime) {
    for (auto& unit : units) {
        unit.update(world, deltaTime);
        if (unit.isWorker()) {
            unit.tryStartCollecting(world);
            player.addResources(unit.updateCollecting(world, deltaTime));
        }
        unit.findTarget(buildings);
        unit.updateAttack(deltaTime);
    }
    for (auto& building : buildings) {
        building.update(deltaTime);
        if (building.productionFinished()) {
            if (building.isBarracks()) {
                float spawnX = building.getX() + building.getWidth() + 20.0f;
                float spawnY = building.getY() + building.getHeight() / 2.0f;
                spawnUnit(spawnX, spawnY, UnitType::Soldier, Team::Player);
                printf(" New soldier spawned \n");
            }
            else if (building.isBase()) {
                float spawnX = building.getX() + building.getWidth() + 20.0f;
                float spawnY = building.getY() + building.getHeight() / 2.0f;
                spawnUnit(spawnX, spawnY, UnitType::Worker, Team::Player);
                printf(" New Worker spawned \n");
            }
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

void EntityManager::spawnUnit(float x, float y, UnitType type, Team team) {
    units.emplace_back(x, y, type, team);
};

void EntityManager::spawnBuilding(float x, float y, BuildingType type, Team team) {
    buildings.emplace_back(x, y, type, team);
};