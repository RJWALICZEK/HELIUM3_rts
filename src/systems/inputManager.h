#pragma once

#include <SDL2/SDL.h>
#include "Camera.h"
#include "EntityManager.h"
#include "HUD.h"
#include "Player.h"
#include "World.h"

class InputManager {
private:
    Camera& camera;
    EntityManager& entities;
    HUD& hud;
    World& world;


    bool isDragging = false;
    //dragSelection
    int dragStartX = 0;
    int dragStartY = 0;
    int dragCurrentX = 0;
    int dragCurrentY = 0;

    void handleRightClick(float worldX, float worldY);

public:
    InputManager(Camera& camera, EntityManager& entities, HUD& hud, World& world);

    void handleEvent(const SDL_Event& event, Player& player);

    void handleMouseButtonDown(int screenX, int screenY);
    void handleMouseButtonUp(int screenX, int screenY);
    void updateDragSelection(int mouseX, int mouseY);
    void renderSelectionBox(SDL_Renderer* renderer);
    bool isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const;

};