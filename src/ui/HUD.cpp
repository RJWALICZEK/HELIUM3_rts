#include "HUD.h"
#include "EntityManager.h"
#include <cstdio>

HUD::HUD() {
    btnProduce.x = 650;
    btnProduce.y = 10;
    btnProduce.w = 140;
    btnProduce.h = 40;
}

void HUD::handleClick(int mouseX, int mouseY, int& resources, EntityManager& entities) {
    if (mouseX >= btnProduce.x && mouseX <= btnProduce.x + btnProduce.w &&
        mouseY >= btnProduce.y && mouseY <= btnProduce.y + btnProduce.h) {
        auto& buildings = entities.getBuildings();

        for (auto& building : entities.getBuildings()) {
            if (building.isBarracks() && resources >= 50 && !building.getProductingStatus()) {
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
void HUD::handleHover(int mouseX, int mouseY) {
    btnProduceHovered = (mouseX >= btnProduce.x && mouseX <= btnProduce.x + btnProduce.w &&
        mouseY >= btnProduce.y && mouseY <= btnProduce.y + btnProduce.h);
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