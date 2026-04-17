#include <cstdio>
#include "inputManager.h"

InputManager::InputManager(Camera& camera, EntityManager& entities, HUD& hud, World& world)
    : camera(camera), entities(entities), hud(hud), world(world) {
};

void InputManager::handleEvent(const SDL_Event& event, Player& player) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            handleMouseButtonDown(event.button.x, event.button.y);
            hud.handleClick(event.button.x, event.button.y, player, entities);  // click on soldier produce btn
            world.handleResourceClick(camera.screenToWorldX(event.button.x), camera.screenToWorldY(event.button.y));
        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            float worldX = camera.screenToWorldX(event.button.x);
            float worldY = camera.screenToWorldY(event.button.y);
            hud.handleClick(worldX, worldY, player, entities);
            handleRightClick(worldX, worldY);
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            handleMouseButtonUp(event.button.x, event.button.y);
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        if (isDragging) {
            updateDragSelection(event.motion.x, event.motion.y);
        }
        hud.handleHover(event.motion.x, event.motion.y);
    }
};
void InputManager::handleMouseButtonDown(int screenX, int screenY)
{
    float worldX = camera.screenToWorldX(screenX);
    float worldY = camera.screenToWorldY(screenY);

    bool clickedOnUnit = false;

    for (auto& unit : entities.getUnits()) {
        if (isClickOnRect(worldX, worldY, unit.getX(), unit.getY(), 28.0f, 28.0f)) {
            if (!(SDL_GetModState() & KMOD_SHIFT)) {
                for (auto& u : entities.getUnits()) u.deselect();
            }
            unit.select();
            clickedOnUnit = true;
            break;
        }
    }

    if (!clickedOnUnit) {
        isDragging = true;
        dragStartX = dragCurrentX = screenX;
        dragStartY = dragCurrentY = screenY;

        if (!(SDL_GetModState() & KMOD_SHIFT)) {
            for (auto& u : entities.getUnits()) u.deselect();
        }
    }
}
void InputManager::handleMouseButtonUp(int screenX, int screenY) {
    if (!isDragging) return;

    float worldStartX = camera.screenToWorldX(dragStartX);
    float worldStartY = camera.screenToWorldY(dragStartY);
    float worldEndX = camera.screenToWorldX(dragCurrentX);
    float worldEndY = camera.screenToWorldY(dragCurrentY);

    float left = std::min(worldStartX, worldEndX);
    float right = std::max(worldStartX, worldEndX);
    float top = std::min(worldStartY, worldEndY);
    float bottom = std::max(worldStartY, worldEndY);

    for (auto& unit : entities.getUnits()) {
        float ux = unit.getX();
        float uy = unit.getY();
        if (ux + 28 > left && ux < right && uy + 28 > top && uy < bottom) {
            unit.select();
        }
    }

    isDragging = false;
};

void InputManager::updateDragSelection(int mouseX, int mouseY) {
    dragCurrentX = mouseX;
    dragCurrentY = mouseY;
};

void InputManager::renderSelectionBox(SDL_Renderer* renderer) {
    if (!isDragging) {
        return;
    }

    //calc box on current display screen
    int x = std::min(dragStartX, dragCurrentX);
    int y = std::min(dragStartY, dragCurrentY);
    int w = std::abs(dragCurrentX - dragStartX);
    int h = std::abs(dragCurrentY - dragStartY);

    //make rectangle box transparent posible
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //halftransparent blue box
    SDL_SetRenderDrawColor(renderer, 0, 129, 255, 70);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);

    //frame
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 180);
    SDL_RenderDrawRect(renderer, &rect);

};
bool InputManager::isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const {
    return (worldMouseX >= objX && worldMouseX <= objX + objW &&
        worldMouseY >= objY && worldMouseY <= objY + objH);
};
void InputManager::handleRightClick(float worldX, float worldY) {


    //unit move to resource
    for (auto& unit : entities.getUnits()) {
        if (unit.isSelected()) {
            unit.moveTo(worldX - 14.0f, worldY - 14.0f); //centered
        }
    }
};