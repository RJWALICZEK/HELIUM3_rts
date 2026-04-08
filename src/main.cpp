#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include "Game.h"



int main(int argc, char* argv[]) {
    Game game;
    game.init();
    game.run();
    game.clean();

    return 0;
}