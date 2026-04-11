#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "Game.h"
#include "Unit.h"
#include "Building.h"

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
    units.emplace_back(300.0f, 300.0f, UnitType::Worker);
    units.emplace_back(500.0f, 300.0f, UnitType::Soldier);
    buildings.emplace_back(50.0f, 50.0f, BuildingType::Base);
    buildings.emplace_back(200.0f, 50.0f, BuildingType::Barracks);

    resourceNodes.push_back({ 300.0f, 400.0f });
    resourceNodes.push_back({ 600.0f, 460.0f });
    resourceNodes.push_back({ 150.0f, 500.0f });

    if (TTF_Init() == -1) {
        printf("RRF_Init error: %s \n", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont("assets/arial.ttf", 18);
    if (!font) {
        printf("failed to load font: %s\n", TTF_GetError());
        font = nullptr; //run game without of font
    }
    printf("font initialized succesfully\n");

    btnProduce.x = 650;
    btnProduce.y = 10;
    btnProduce.w = 140;
    btnProduce.h = 40;


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
                handleMouseClick(event.button.x, event.button.y);  //select unit
                handleHUDClick(event.button.x, event.button.y);
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                handleRightClick(event.button.x, event.button.y);       //move unit
                handleBuildingClick(event.button.x, event.button.y);    //start production process
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            handleMouseHover(event.motion.x, event.motion.y);
        }
    }
}
// Game logic placeholder
void Game::update()
{

    for (auto& unit : units) {
        unit.update(deltaTime);
        //automatic soldier attack
        if (!unit.isSelected() && unit.getType() == UnitType::Soldier) {
            for (auto& building : buildings) {
                if (unit.isInRange(building)) {
                    unit.attack(building, deltaTime);
                    break;
                }
            }
        }
    }

    static float collectTimer = 0.0f;
    collectTimer += deltaTime;

    if (collectTimer >= 4.0f) {
        for (auto& unit : units) {
            if (unit.getType() != UnitType::Worker) continue;

            for (auto& node : resourceNodes) {
                if (!node.active) continue;

                float dx = node.x + 16.0f - unit.getX();
                float dy = node.y + 16.0f - unit.getY();
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < 25.0f) {                    // Robotnik jest przy surowcu
                    if (node.amount > 0) {
                        node.amount -= 7;
                        resources += 7;
                        printf("Worcer colecting helium... %d left\n", node.amount);

                        if (node.amount <= 0) {
                            node.active = false;
                            printf("Helium3 has been depleted\n");
                        }
                    }
                    break;   // jeden Robotnik zbiera z jednego źródła naraz
                }
            }
        }
        collectTimer = 0.0f;
    }

    for (auto& building : buildings) {
        building.update(deltaTime);
        if (building.productionFinished() && building.isBarracks()) {
            float spawnX = building.getX() + building.getWidth() + 20.0f;
            float spawnY = building.getY() + building.getHeight() / 2.0f;
            units.emplace_back(spawnX, spawnY, UnitType::Soldier);
            printf(" New soldier spawned \n");
        }
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
    //vertical stripes
    for (int x = 0; x <= MAP_WIDTH; ++x) {
        SDL_RenderDrawLine(renderer,
            x * TILE_SIZE, 0,
            x * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);
    }
    //horizontal stripes
    for (int y = 0; y <= MAP_HEIGHT; ++y) {
        SDL_RenderDrawLine(renderer,
            0, y * TILE_SIZE, MAP_WIDTH * TILE_SIZE,
            y * TILE_SIZE);
    }
    //display buildings
    for (auto& building : buildings) {
        building.render(renderer);
    }
    //display resources
    renderResources();
    //display units
    for (auto& unit : units) {
        unit.render(renderer);
    }
    //displayHUD
    renderHUD();
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
    handleHUDClick(mouseX, mouseY);
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
    /*if (!anySelected) {
        for (auto& building : buildings) {
            if (mouseX >= building.getX() && mouseX <= building.getX() + building.getWidth() &&
                mouseY >= building.getY() && mouseY <= building.getY() + building.getHeight()) {

                building.takeDamage(300);
                printf("Building take damage %d\n", building.getHP());
                anySelected = true;
                break;
            }
        }

} */
    if (!anySelected) {
        printf(" Click on empty field, unselect everything \n");
    }
}
void Game::handleRightClick(int mouseX, int mouseY) {
    bool anySelected = false;

    handleResourceClick(mouseX, mouseY);

    for (auto& unit : units) {
        if (unit.isSelected()) {
            unit.moveTo(static_cast<float>(mouseX - 12.0f), static_cast<float>(mouseY - 12.0f));
            anySelected = true;
        }
    }

    if (!anySelected) {
        printf(" Nothing selected \n");
    }
}

void Game::handleBuildingClick(int mouseX, int mouseY) {
    for (auto& building : buildings) {
        //click on barracks cheeck
        if (building.isBarracks()) {
            if (mouseX >= building.getX() && mouseX <= building.getX() + building.getWidth() &&
                mouseY >= building.getY() && mouseY <= building.getY() + building.getHeight()) {
                building.startProduction();
                return;
            }
        }
    }
}

void Game::checkGameOver() {
    for (const auto& building : buildings) {
        if (building.getHP() <= 0 && !building.isBarracks()) {
            printf(" \n GAME OVER main base has been destroyed");
            isRunning = false;
            return;
        }
    }
}

void Game::renderHUD() {
    if (!font) {
        return;
    }

    //bottom resource view
    char resText[64];
    sprintf(resText, "Helium-3: %i  |  (+%i/s)", resources, resourcesIncome);

    SDL_Surface* surface = TTF_RenderText_Solid(font, resText, textColor);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_Rect dest = { 10, 10, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }

    // info about selected unit

    for (const auto& unit : units) {
        if (unit.isSelected()) {
            char unitText[64];
            sprintf(unitText, "Selected: %s",
                (unit.getType() == UnitType::Worker) ? "Worker" : "Soldier");

            SDL_Surface* uSurface = TTF_RenderText_Solid(font, unitText, textColor);
            if (uSurface) {
                SDL_Texture* uTexture = SDL_CreateTextureFromSurface(renderer, uSurface);
                if (uTexture) {
                    SDL_Rect dest = { 10, 40, uSurface->w, uSurface->h };
                    SDL_RenderCopy(renderer, uTexture, nullptr, &dest);
                    SDL_DestroyTexture(uTexture);
                }
                SDL_FreeSurface(uSurface);
            }

            break;   // for now , first unit select info
        }
    }
    //btn "produce soldier" in barracks
    if (btnProduceHovered) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
    }
    SDL_RenderFillRect(renderer, &btnProduce);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btnProduce);

    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Produce Soldier", textColor);
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                SDL_Rect textRect = {
                    btnProduce.x + 10,
                    btnProduce.y + 10,
                    surf->w,
                    surf->h
                };
                SDL_RenderCopy(renderer, tex, nullptr, &textRect);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }
    }
}

void Game::handleHUDClick(int mouseX, int mouseY) {
    if (mouseX >= btnProduce.x && mouseX <= btnProduce.x + btnProduce.w &&
        mouseY >= btnProduce.y && mouseY <= btnProduce.h) {
        for (auto& building : buildings) {
            if (building.isBarracks() && resources >= 50) {
                building.startProduction();
                printf("production start from button");
                resources -= 50;
                return;
            }
            else {
                printf("not enough helium3");
            }
        }
    }
}

void Game::handleMouseHover(int mouseX, int mouseY) {
    btnProduceHovered = (mouseX >= btnProduce.x && mouseX <= btnProduce.x + btnProduce.w &&
        mouseY >= btnProduce.y && mouseY <= btnProduce.h);
}

void Game::renderResources() { //yellow rect , resource
    for (const auto& node : resourceNodes) {
        if (!node.active) { continue; }

        SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);
        SDL_Rect rect = {
            static_cast<int>(node.x),
            static_cast<int>(node.y),
            32,32
        };
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 200, 180, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Game::handleResourceClick(int mouseX, int mouseY) {
    for (auto& node : resourceNodes) {
        if (!node.active) { continue; }

        if (mouseX >= node.x && mouseX <= node.x + 32 &&
            mouseY >= node.y && mouseY <= node.y + 32) {

            for (auto& unit : units) {
                if (unit.isSelected() && unit.getType() == UnitType::Worker) {
                    unit.moveTo(node.x + 8.0f, node.y + 8.0f);
                    printf("Worken colecting resource\n");
                }
            }
            return;
        }
    }
}