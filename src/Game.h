#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Unit.h"
#include "Building.h"
#include "Camera.h"
#include "EntityManager.h"

class Unit;
class Building;
class Game {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Rect btnProduce;
    SDL_Rect selectionBox;
    Camera camera;
    EntityManager entities;

    bool isRunning = false;
    bool btnProduceHovered = false;

    Uint32 lastTime = 0;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = 0;
    Uint32 fps = 0;

    int resources = 50;
    int resourcesIncome = 0;

    static constexpr int WORLD_WIDTH = 2400;
    static constexpr int WORLD_HEIGHT = 1800;




    float deltaTime = 0.0f;

    const int TILE_SIZE = 32;
    const int MAP_WIDTH = 25; //25x32=800px
    const int MAP_HEIGHT = 19;  //19x32=608px


    struct ResourceNode {
        float x;
        float y;
        int amount = 4700;
        bool active = true;
    };
    std::vector<ResourceNode> resourceNodes;

    void handleEvents();
    void update();
    void render();
    void handleMouseClick(int mouseX, int mouseY);
    void handleRightClick(int mouseX, int mouseY);
    void handleBuildingClick(int mouseX, int mouseY);
    void handleMouseHover(int mouseX, int mouseY);
    void handleHUDClick(int mouseX, int mouseY);
    void checkGameOver();
    void renderHUD();
    void renderResources(float camX, float camY); //???? check it later
    void handleResourceClick(float worldX, float worldY);


    void handleMouseButtonDown(int mouseX, int mouseY);
    void handleMouseButtonUp(int mouseX, int mouseY);
    void updateDragSelection(int mouseX, int mouseY);
    void renderSelectionBox();

    bool isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const;
    bool isDragging = false;
    int dragStartX = 0;
    int dragStartY = 0;
    int dragCurrentX = 0;
    int dragCurrentY = 0;
public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};