#pragma once
#include <SDL2/SDL.h>

class Game {
    private:
    void handleEvents();
    void update();
    void render();
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning = false;
    public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};