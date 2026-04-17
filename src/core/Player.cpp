#include <cstdio>
#include "Player.h"

void Player::update(float deltaTime) {
    (void)deltaTime;
}
void Player::addResources(int amount) {
    if (amount > 0) {
        resources += amount;
    }
}
bool Player::spendResources(int amount) {
    if (amount <= 0) {
        return true;
    }
    if (resources >= amount) {
        resources -= amount;
        return true;
    }
    else {
        printf(" Not enough resources...\n");
        return false;
    }
}
void Player::setResourcesIncome(int income) {
    if (income >= 0) {
        resourcesIncome = income;
    }
}
