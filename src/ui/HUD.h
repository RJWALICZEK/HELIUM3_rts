#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class HUD {
private:
    SDL_Rect btnProduceSoldier;
    SDL_Rect btnProduceWorker;
    SDL_Rect btnProduceUnit;
    bool btnSoldierHovered = false;
    bool btnWorkerHovered = false;
    bool btnUnitHovered = false;
public:
    HUD();
    void handleClick(int mouseX, int mouseY, class Player& player, class EntityManager& entities);
    void handleHover(int mouseX, int mouseY);

    void render(SDL_Renderer* renderer, TTF_Font* font, int resources, int resourcesIncome, EntityManager& entities);

};

//from game.h
/*
    void renderHUD();
    void handleHUDClick(int mouseX, int mouseY);
    SDL_Rect btnProduce;
    bool btnProduceHovered = false;

*/