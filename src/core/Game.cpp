#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <vector>
#include <cmath>
#include "Game.h"

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
    if (TTF_Init() == -1) {
        printf("RRF_Init error: %s \n", TTF_GetError());
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
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                hud.handleClick(event.button.x, event.button.y, player, entities);  // click on soldier produce btn
                handleMouseButtonDown(event.button.x, event.button.y);
                world.handleResourceClick(camera.screenToWorldX(event.button.x), camera.screenToWorldY(event.button.y));
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                handleRightClick(event.button.x, event.button.y);
                float worldX = camera.screenToWorldX(event.button.x);
                float worldY = camera.screenToWorldY(event.button.y);
                hud.handleClick(worldX, worldY, player, entities);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                handleMouseButtonUp(event.button.x, event.button.y);
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            if (isDragging) {
                updateDragSelection(event.motion.x, event.motion.y);
            }
            hud.handleHover(event.motion.x, event.motion.y);
        }
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

    renderSelectionBox();
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

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

 /*
void Game::handleMouseClick(int mouseX, int mouseY) {
    float worldX = camera.screenToWorldX(mouseX);
    float worldY = camera.screenToWorldY(mouseY);
    bool anySelected = false;
    //click on unit rectangle checker
    for (auto& unit : entities.getUnits()) {
        if (isClickOnRect(worldX, worldY, unit.getX(), unit.getY(), 28.0f, 28.0f)) {
            unit.select();
            anySelected = true;
        }
        else {
            unit.deselect();
        }
    }
    // unit attack by click
   if (!anySelected) {
        for (auto& building : buildings) {
            if (mouseX >= building.getX() && mouseX <= building.getX() + building.getWidth() &&
                mouseY >= building.getY() && mouseY <= building.getY() + building.getHeight()) {

                building.takeDamage(70);
                printf("Building take damage %d\n", building.getHP());
                anySelected = true;
                break;

            }
        }

    }*
    if (!anySelected) {
        printf(" Click on empty field, unselect everything \n");
    }
}


void Game::handleBuildingClick(int mouseX, int mouseY) {
    float worldX = camera.screenToWorldX(mouseX);
    float worldY = camera.screenToWorldY(mouseY);
    for (auto& building : entities.getBuildings()) {

        //click on barracks cheeck
        if (building.isBarracks() && resources >= 50 && !building.getProductingStatus()) {
            if (isClickOnRect(worldX, worldY, building.getX(), building.getY(), building.getWidth(), building.getHeight())) {
                building.startProduction();
                resources -= 50;
                return;
            }
        }
    }
}






 */
bool Game::isClickOnRect(float worldMouseX, float worldMouseY, float objX, float objY, float objW, float objH) const {
    return (worldMouseX >= objX && worldMouseX <= objX + objW &&
        worldMouseY >= objY && worldMouseY <= objY + objH);
}

void Game::handleMouseButtonDown(int mouseX, int mouseY) {
    float worldX = camera.screenToWorldX(mouseX);
    float worldY = camera.screenToWorldY(mouseY);

    //check of unit click
    bool clickedOnUnit = false;
    for (auto& unit : entities.getUnits()) {
        if (isClickOnRect(worldX, worldY, unit.getX(), unit.getY(), 28.0f, 28.0f)) {
            //in no shift btn , uncheck all units
            if (!(SDL_GetModState() & KMOD_SHIFT)) {
                for (auto& u : entities.getUnits()) {
                    u.deselect();
                }
            }
            unit.select();
            clickedOnUnit = true;
            break;
        }
    }
    if (!clickedOnUnit) {
        //drag selection mode
        isDragging = true;
        dragStartX = mouseX;
        dragStartY = mouseY;
        dragCurrentX = mouseX;
        dragCurrentY = mouseY;
        //uchecked all units in no shift btn
        if (!(SDL_GetModState() & KMOD_SHIFT)) {
            for (auto& u : entities.getUnits()) {
                u.deselect();
            }
        }
    }
    if (!isDragging)
    {
        hud.handleHover(mouseX, mouseY);
    }
}

void Game::handleMouseButtonUp(int mouseX, int mouseY) {
    if (!isDragging) {
        return;
    }

    float worldStartX = camera.screenToWorldX(dragStartX);
    float worldStartY = camera.screenToWorldY(dragStartY);
    float worldEndX = camera.screenToWorldX(mouseX);
    float worldEndY = camera.screenToWorldY(mouseY);

    // normalize draging check box
    float left = std::min(worldStartX, worldEndX);
    float right = std::max(worldStartX, worldEndX);
    float top = std::min(worldStartY, worldEndY);
    float bottom = std::max(worldStartY, worldEndY);

    //checking units inside box
    for (auto& unit : entities.getUnits()) {
        float ux = unit.getX();
        float uy = unit.getY();

        if (ux + 28 > left && ux < right && uy + 28 > top && uy < bottom) {
            unit.select();
        }
    }
    isDragging = false;

}

void Game::updateDragSelection(int mouseX, int mouseY) {
    dragCurrentX = mouseX;
    dragCurrentY = mouseY;
}

void Game::renderSelectionBox() {
    if (!isDragging) {
        return;
    }

    //calc box on current display screen
    int x = std::min(dragStartX, dragCurrentX);
    int y = std::min(dragStartY, dragCurrentY);
    int w = std::abs(dragCurrentX - dragStartX);
    int h = std::abs(dragCurrentY - dragStartY);

    //make rectangle box transparent posible
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //halftransparent blue box
    SDL_SetRenderDrawColor(renderer, 0, 129, 255, 70);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);

    //frame
    SDL_SetRenderDrawColor(renderer, 0, 180, 255, 180);
    SDL_RenderDrawRect(renderer, &rect);

}


void Game::handleRightClick(int mouseX, int mouseY) {
    float worldX = camera.screenToWorldX(mouseX);
    float worldY = camera.screenToWorldY(mouseY);
    bool anySelected = false;

    //unit move to resource
    for (auto& unit : entities.getUnits()) {
        if (unit.isSelected()) {
            unit.moveTo(worldX - 14.0f, worldY - 14.0f); //centered
            anySelected = true;
        }
    }

    if (!anySelected) {
        printf(" Nothing selected \n");
    }
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