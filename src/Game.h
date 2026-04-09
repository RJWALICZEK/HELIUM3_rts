#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <string>
#include "Unit.h"
class Unit;
class Game {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool isRunning = false;

    Uint32 lastTime = 0;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = 0;
    Uint32 fps = 0;

    float deltaTime = 0.0f;

    const int TITLE_SIZE = 32;
    const int MAP_WIDTH = 25; //25x32=800px
    const int MAP_HEIGHT = 19;  //19x32=608px

    std::vector<Unit> units;

    void handleEvents();
    void update();
    void render();
    void handleMouseClick(int mouseX, int mouseY);
    void handleRightClick(int mouseX, int mouseY);
public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};