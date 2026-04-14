#pragma once

#include <vector>
#include <SDL2/SDL.h>

struct ResourceNode {
    float x;
    float y;
    int amount = 4700;
    bool active = true;
};

class World {
private:
    std::vector<ResourceNode> resourceNodes;
    const int TILE_SIZE = 32;

public:
    World() = default;
    void init();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, float camx, float camY);
    void handleResourceClick(float worldX, float worldY);

    std::vector<ResourceNode>& getResourceNodes() { return resourceNodes; }
    const std::vector<ResourceNode>& getResourceNodes() const { return resourceNodes; }

    int getTileSize() const { return TILE_SIZE; }
};