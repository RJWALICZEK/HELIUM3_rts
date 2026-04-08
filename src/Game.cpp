#include <cstdio>
#include "Game.h"
#include <stdio.h>
#include <SDL2/SDL_video.h>

Game::Game() = default;
Game::~Game() = default;
// SDL/windows initialization,
bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
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
    if (!window)
    {
        printf("Create window error %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("Renderer error. %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    isRunning = true;
    printf("HELIUM3 is running successfully.\n");

    lastTime = SDL_GetTicks();
    return true;
}
// Event handler
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
    }
}
// Game logic placeholder
void Game::update()
{
}
// screen cleaning and background drawing
void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 75, 0, 255);
    SDL_RenderClear(renderer);

    //for now only background , later units and map

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
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
    printf("Program HELIUM3 terminated successfully.\n");
}