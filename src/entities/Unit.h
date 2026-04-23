#pragma once
#include <SDL2/SDL.h>

class Building;

enum class UnitType {
    Worker,
    Soldier
};

class Unit {
private:
    float posX;
    float posY;

    float targetX = 0.0f;
    float targetY = 0.0f;
    float speed = 100.0f;
    float attackTimer = 0.0f;
    float attackCooldown = 1.0f;
    float collectTimer = 0.0f;
    bool isCollecting = false;
    int attackDmg = 100;


    bool isMoving = false;
    bool selected = false;
    bool isAttacking = false;

    UnitType type;

public:

    Unit(float x, float y, UnitType type);
    ~Unit() = default;

    void update(class World& world, float deltaTime);
    void render(SDL_Renderer* renderer, float camX, float camY);

    float getX() const { return posX; };
    float getY() const { return posY; };
    UnitType getType() const { return type; };

    void select();
    void deselect();
    bool isSelected() const;

    void moveTo(float targetX, float targetY);
    void setSpeed(float newSpeed);

    void attack(Building& target, float deltaTime);
    bool isInRange(const Building& target) const;

    void tryStartCollecting(class World& world);
    bool isWorker() const;

    int updateCollecting(class World& world, float deltaTime);
};