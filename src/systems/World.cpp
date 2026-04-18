#include "World.h"
#include <cstdio>
#include <cmath>


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

        SDL_SetRenderDrawColor(renderer, 231, 0, 245, 255);
        SDL_Rect rect = { screenX, screenY, 32, 32 };
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 243, 143, 246, 255);
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
bool World::isNearResource(float unitX, float unitY, float& outNodeX, float& outNodeY) {
    for (const auto& node : resourceNodes) {
        float dx = node.x + 16.0f - unitX;
        float dy = node.y + 16.0f - unitY;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 30.0f) {
            outNodeX = node.x;
            outNodeY = node.y;
            return true;
        }
    }
    return false;
}

int World::takeResource(float nodeX, float nodeY, int amount) {
    for (auto& node : resourceNodes) {
        if (!node.active) { continue; }

        float dx = node.x + 16.f - nodeX;
        float dy = node.y + 16.f - nodeY;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 45.0f) {
            int taken = std::min(node.amount, amount);
            node.amount -= taken;
            if (node.amount <= 0) {
                node.active = false;
                printf("Heloum3 node depleted at %.1f | %.1f \n", node.x, node.y);
            }
            return taken;
        }
    }
    return 0;

}


