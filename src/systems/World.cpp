#include "World.h"
#include <cstdio>


void World::init() {
    //start world resources
    resourceNodes.push_back({ 300.0f, 400.0f });
    resourceNodes.push_back({ 600.0f, 460.0f });
    resourceNodes.push_back({ 150.0f, 500.0f });

    printf("World initialized - resource nodes (%zu) created\n", resourceNodes.size());
}
void World::update(float deltaTime) {
    (void)deltaTime;
}

void World::render(SDL_Renderer* renderer, float camX, float camY) {
    if (!renderer) {
        return;
    }

    for (const auto& node : resourceNodes) {
        if (!node.active) {
            continue;
        }

        int screenX = static_cast<int>(node.x - camX);
        int screenY = static_cast<int>(node.y - camY);

        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        SDL_Rect rect = { screenX, screenY, 32, 32 };
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 200, 180, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }

}

void World::handleResourceClick(float worldX, float worldY) {
    for (auto& node : resourceNodes) {
        if (!node.active) { continue; }

        if (worldX >= node.x && worldX <= node.x + 32 &&
            worldY >= node.y && worldY <= node.y + 32) {

            printf("Resource (%1.f, %1.f) - %d left\n", node.x, node.y, node.amount);
            //worker colect resources here
            return;
        }
    }
}
