#pragma once
#include "Game.h"

int main() {

    Game game("Fish.exe", 640, 484, 4, 5, false);


    // MAIN PROGRAM LOOP
    while (!game.getWindowShouldClose()) { // As long as window is open

        game.update();

        game.render();
 
    }

    // Delete program
    return 0;
}