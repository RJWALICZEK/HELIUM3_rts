#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include "Camera.h"
#include "EntityManager.h"
#include "HUD.h"
#include "World.h"


class Game {
private:

    ///////////////////////////////////////////////
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    Camera camera;
    EntityManager entities;
    HUD hud;
    World world;


    bool isRunning = false;
    bool isDragging = false;

    Uint32 lastTime = 0;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = 0;
    Uint32 fps = 0;

    int resources = 50;
    int resourcesIncome = 0;

    float deltaTime = 0.0f;

    const int TILE_SIZE = 32;
    SDL_Rect selectionBox;
    //dragSelection
    int dragStartX = 0;
    int dragStartY = 0;
    int dragCurrentX = 0;
    int dragCurrentY = 0;
    //world resources

    //private methods
    void handleEvents();
    void update();
    void render();

    bool isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const;
    void handleMouseButtonDown(int mouseX, int mouseY);
    void handleMouseButtonUp(int mouseX, int mouseY);
    void handleRightClick(int mouseX, int mouseY);

    void updateDragSelection(int mouseX, int mouseY);
    void renderSelectionBox();


    void renderResources(float camX, float camY); //???? check it later

    void checkGameOver();

public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};

/*OLD
    const int TILE_SIZE = 32;
    const int MAP_WIDTH = 25; //25x32=800px
    const int MAP_HEIGHT = 19;  //19x32=608px




    void handleMouseClick(int mouseX, int mouseY);
    void handleBuildingClick(int mouseX, int mouseY);
    void renderResources(float camX, float camY); //???? check it later




    bool isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const;
*/