#include <SDL2/SDL_video.h>
#include <cstdio>
#include <stdio.h>
#include <vector>
#include "Game.h"
#include "Unit.h"

Game::Game() = default;
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

    isRunning = true;
    printf(" HELIUM3 is running successfully.\n");

    lastTime = SDL_GetTicks();
    units.emplace_back(400.0f, 300.0f, UnitType::Worker);
    return true;
}
// Event handler
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                handleMouseClick(event.button.x, event.button.y);
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                handleRightClick(event.button.x, event.button.y);
            }
        }
    }
}
// Game logic placeholder
void Game::update()
{
    for (auto& unit : units) {
        unit.update(deltaTime);
    }
}
// screen cleaning and background drawing
void Game::render()
{
    // Clearing screen
    SDL_SetRenderDrawColor(renderer, 0, 75, 0, 255);
    SDL_RenderClear(renderer);

    //map grid overlay (green stripes)
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    //vertical stripes
    for (int x = 0; x <= MAP_WIDTH; ++x) {
        SDL_RenderDrawLine(renderer,
            x * TITLE_SIZE, 0,
            x * TITLE_SIZE, MAP_HEIGHT * TITLE_SIZE);
    }
    //horizontal stripes
    for (int y = 0; y <= MAP_HEIGHT; ++y) {
        SDL_RenderDrawLine(renderer,
            0, y * TITLE_SIZE, MAP_WIDTH * TITLE_SIZE,
            y * TITLE_SIZE);
    }
    for (auto& unit : units) {
        unit.render(renderer);
    }
    //display screen
    SDL_RenderPresent(renderer);

    // fps counter in window tittle, 

    char title[64];
    sprintf(title, "HELIUM3 v0.1 - FPS: %u", fps);
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
    SDL_Quit();
    printf(" Program HELIUM3 terminated successfully.\n");
}

void Game::handleMouseClick(int mouseX, int mouseY) {
    bool anySelected = false;
    //click on unit rectangle checker
    for (auto& unit : units) {
        if (mouseX >= unit.getX() && mouseX <= unit.getX() + 28 &&
            mouseY >= unit.getY() && mouseY <= unit.getY() + 28) {
            unit.select();
            anySelected = true;
        }
        else {
            unit.deselect();
        }
    }

    if (!anySelected) {
        printf(" Click on empty field, unselect everything \n");
    }
}
void Game::handleRightClick(int mouseX, int mouseY) {
    bool anySelected = false;

    for (auto& unit : units) {
        if (unit.isSelected()) {
            unit.moveTo(static_cast<float>(mouseX), static_cast<float>(mouseY));
            anySelected = true;
        }
    }

    if (!anySelected) {
        printf(" Nothing selected \n");
    }
}