#pragma once

enum class Team {
    Player,
    Enemy
};

enum class UnitType {
    Worker,
    Soldier
};

enum class BuildingType {
    Base,
    Barracks
};
enum class UnitState {
    Idle,
    Moving,
    Attacking,
    Gathering
};