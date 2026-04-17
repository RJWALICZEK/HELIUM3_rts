#pragma once

class Player {
private:
    int resources = 50;
    int resourcesIncome = 0;

public:
    Player() = default;

    void update(float deltaTime);

    int getResources() const { return resources; }
    int getResourcesIncome() const { return resourcesIncome; }

    void addResources(int amount);
    bool spendResources(int amount);
    void setResourcesIncome(int income);
};