#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include "Camera.h"
#include "EntityManager.h"
#include "HUD.h"
#include "Player.h"
#include "World.h"
#include "InputManager.h"



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
    Player player;
    InputManager inputManager;

    bool isRunning = false;

    Uint32 lastTime = 0;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = 0;
    Uint32 fps = 0;


    float deltaTime = 0.0f;

    const int TILE_SIZE = 32;

    void handleEvents();
    void update();
    void render();

    void checkGameOver();

public:
    Game();
    bool init();
    void run();
    void clean();
    ~Game();
};