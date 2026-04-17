#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <vector>
#include <cmath>
#include "Game.h"
#include "inputManager.h"


Game::Game()
    : inputManager(camera, entities, hud, world) {
};
Game::~Game() = default;
// SDL/windows initialization,
bool Game::init()
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(
        "HELIUM3 v0.1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Create window error %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Renderer error. %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init error: %s \n", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont("assets/arial.ttf", 18);
    if (!font) {
        printf("failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        font = nullptr; //run game without of font
    }

    isRunning = true;
    lastTime = SDL_GetTicks();


    entities.init();
    world.init();


    //center camera on main base at start
    const auto& buildings = entities.getBuildings();
    if (!buildings.empty()) {
        const auto& base = buildings[0];
        camera.setPosition(base.getX() - 300.0f, base.getY() - 200.0f);
    }
    printf(" HELIUM3 is running successfully.\n");
    return true;
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        inputManager.handleEvent(event, player);
    }
}
void Game::update()
{

    entities.update(deltaTime);
    camera.update(deltaTime);

    static float collectTimer = 0.0f;
    collectTimer += deltaTime;


    if (collectTimer >= 4.0f) {
        for (auto& unit : entities.getUnits()) {
            if (unit.getType() != UnitType::Worker) continue;

            for (auto& node : world.getResourceNodes()) {
                if (!node.active) continue;

                float dx = node.x + 16.0f - unit.getX();
                float dy = node.y + 16.0f - unit.getY();
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < 25.0f) {                    // worker is near resource
                    if (node.amount > 0) {
                        node.amount -= 7;
                        player.addResources(7);
                        printf("Worcer colecting helium... %d left\n", node.amount);

                        if (node.amount <= 0) {
                            node.active = false;
                            printf("Helium3 has been depleted\n");
                        }
                    }
                    break;   // one worker collect from one resource in the same time
                }
            }
        }
        collectTimer = 0.0f;
    }



    checkGameOver();
}
// screen cleaning and background drawing
void Game::render()
{
    // Clearing screen
    SDL_SetRenderDrawColor(renderer, 0, 75, 0, 255);
    SDL_RenderClear(renderer);

    //map grid overlay (green stripes)
    SDL_SetRenderDrawColor(renderer, 8, 51, 4, 255);

    int startX = static_cast<int>(camera.getX() / TILE_SIZE) * TILE_SIZE;
    int startY = static_cast<int>(camera.getY() / TILE_SIZE) * TILE_SIZE;
    //vertical stripes
    for (int x = startX; x < camera.getX() + 800 + TILE_SIZE; x += TILE_SIZE) {
        int screenX = static_cast<int>(x - camera.getX());
        SDL_RenderDrawLine(renderer,
            screenX, 0,
            screenX, 600);
    }
    //horizontal stripes
    for (int y = startY; y < camera.getY() + 600 + TILE_SIZE; y += TILE_SIZE) {
        int screenY = static_cast<int>(y - camera.getY());
        SDL_RenderDrawLine(renderer,
            0, screenY,
            800, screenY);
    }

    //display resources
    world.render(renderer, camera.getX(), camera.getY());
    //display units
    entities.render(renderer, camera.getX(), camera.getY());
    //unit selection box
    inputManager.renderSelectionBox(renderer);
    //displayHUD
    hud.render(renderer, font, player.getResources(), player.getResourcesIncome(), entities);
    //display screen
    SDL_RenderPresent(renderer);

    // fps counter in window tittle, 

    char title[128];
    sprintf(title, "HELIUM3 v0.1 - FPS: %u  | camera: %.0f , %.0f", fps, camera.getX(), camera.getY());
    SDL_SetWindowTitle(window, title);

}
// game main loop
void Game::run()
{
    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        frameCount++;
        if (currentTime - fpsTimer >= 1000) {
            fps = frameCount;
            frameCount = 0;
            fpsTimer = currentTime;
        }

        handleEvents();
        update();
        render();
    }
}
// SDL resources cleaner
void Game::clean()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
    printf(" Program HELIUM3 terminated successfully.\n");
}


void Game::checkGameOver() {
    for (const auto& building : entities.getBuildings()) {
        if (building.getHP() <= 0 && !building.isBarracks()) {
            printf(" \n GAME OVER main base has been destroyed");
            isRunning = false;
            return;
        }
    }
}