#pragma once
#include <SDL2/SDL.h>
#include <string>

class Game {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning = false;
    Uint32 lastTime = 0;
    float deltaTime = 0.0f;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = 0;
    Uint32 fps = 0;
    const int TITLE_SIZE = 32;
    const int MAP_WIDTH = 25; //25x32=800px
    const int MAP_HEIGHT = 19;  //19x32=608px
    void handleEvents();
    void update();
    void render();
public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};