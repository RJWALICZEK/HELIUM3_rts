#pragma once
#include <SDL2/SDL.h>
#include <String>

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