#include "HUD.h"
#include "EntityManager.h"
#include "Player.h"
#include <cstdio>

HUD::HUD() {

    const int startX = 650;
    const int startY = 10;
    const int btnWidth = 140;
    const int btnHeight = 40;
    const int spacing = 50;

    btnProduceSoldier.x = startX;
    btnProduceSoldier.y = startY;
    btnProduceSoldier.w = btnWidth;
    btnProduceSoldier.h = btnHeight;

    btnProduceWorker.x = startX;
    btnProduceWorker.y = startY + spacing;
    btnProduceWorker.w = btnWidth;
    btnProduceWorker.h = btnHeight;

    btnProduceUnit.x = startX;
    btnProduceUnit.y = startY + spacing * 2;
    btnProduceUnit.w = btnWidth;
    btnProduceUnit.h = btnHeight;

}

void HUD::handleClick(int mouseX, int mouseY, Player& player, EntityManager& entities) {
    if (mouseX >= btnProduceWorker.x && mouseX <= btnProduceWorker.x + btnProduceWorker.w &&
        mouseY >= btnProduceWorker.y && mouseY <= btnProduceWorker.y + btnProduceWorker.h) {

        for (auto& building : entities.getBuildings()) {
            if (building.isBase() && !building.getProductingStatus()) {
                if (player.spendResources(50)) {
                    printf("start production worker button clicked\n");
                    building.startProduction();
                    return;
                }
                else { //might be a problem when i add more barracks, slove it later
                    printf("Not enough helium3...\n");
                    break;
                }
            }
        }
    }
    if (mouseX >= btnProduceSoldier.x && mouseX <= btnProduceSoldier.x + btnProduceSoldier.w &&
        mouseY >= btnProduceSoldier.y && mouseY <= btnProduceSoldier.y + btnProduceSoldier.h) {

        for (auto& building : entities.getBuildings()) {
            if (building.isBarracks() && !building.getProductingStatus()) {
                if (player.spendResources(50)) {
                    printf("start produce soldier button clicked\n");
                    building.startProduction();
                    return;
                }
                else { //might be a problem when i add more barracks, slove it later
                    printf("Not enough helium3...\n");
                    break;
                }
            }
        }
    }
}
void HUD::handleHover(int mouseX, int mouseY) {
    btnSoldierHovered = (mouseX >= btnProduceSoldier.x && mouseX <= btnProduceSoldier.x + btnProduceSoldier.w &&
        mouseY >= btnProduceSoldier.y && mouseY <= btnProduceSoldier.y + btnProduceSoldier.h);
    btnWorkerHovered = (mouseX >= btnProduceWorker.x && mouseX <= btnProduceWorker.x + btnProduceWorker.w &&
        mouseY >= btnProduceWorker.y && mouseY <= btnProduceWorker.y + btnProduceWorker.h);
    btnUnitHovered = (mouseX >= btnProduceUnit.x && mouseX <= btnProduceUnit.x + btnProduceUnit.w &&
        mouseY >= btnProduceUnit.y && mouseY <= btnProduceUnit.y + btnProduceUnit.h);
}

void HUD::render(SDL_Renderer* renderer, TTF_Font* font, int resources, int resourcesIncome, EntityManager& entities) {
    if (!font) {
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 };
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
    for (const auto& unit : entities.getUnits()) {
        if (unit.isSelected()) {
            char unitText[64];
            sprintf(unitText, "Selec ted: %s",
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


    //buttons//////////////////////////////

    if (btnWorkerHovered) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
    }
    SDL_RenderFillRect(renderer, &btnProduceWorker);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btnProduceWorker);

    SDL_Surface* surfWorker = TTF_RenderText_Solid(font, "Produce Worker", textColor);
    if (surfWorker) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surfWorker);
        if (tex) {
            SDL_Rect textRect = {
                btnProduceWorker.x + 10,
                btnProduceWorker.y + 10,
                surfWorker->w,
                surfWorker->h
            };
            SDL_RenderCopy(renderer, tex, nullptr, &textRect);
            SDL_DestroyTexture(tex);
        }
        SDL_FreeSurface(surfWorker);
    }


    ////btn Soldier ////////////////
    if (btnSoldierHovered) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
    }
    SDL_RenderFillRect(renderer, &btnProduceSoldier);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btnProduceSoldier);

    SDL_Surface* surfSoldier = TTF_RenderText_Solid(font, "Produce Soldier", textColor);
    if (surfSoldier) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surfSoldier);
        SDL_Rect textRect = { btnProduceSoldier.x + 10, btnProduceSoldier.y + 10, surfSoldier->w, surfSoldier->h };
        SDL_RenderCopy(renderer, tex, nullptr, &textRect);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surfSoldier);
    }
    ///btn un////
    if (btnUnitHovered) {
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
    }
    SDL_RenderFillRect(renderer, &btnProduceUnit);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btnProduceUnit);

    SDL_Surface* surfUnit = TTF_RenderText_Solid(font, "Produce Unit", textColor);
    if (surfUnit) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surfUnit);
        SDL_Rect textRect = { btnProduceUnit.x + 10, btnProduceUnit.y + 10, surfUnit->w, surfUnit->h };
        SDL_RenderCopy(renderer, tex, nullptr, &textRect);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surfUnit);
    }

}