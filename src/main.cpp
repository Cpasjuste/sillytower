//
// Created by cpasjuste on 08/12/16.
//

#include "game.h"

int main(int argc, char *argv[]) {

    auto game = new Game();

    while (true) {

        unsigned int keys = game->getInput()->getKeys();
        if (keys & EV_QUIT) {
            break;
        }

        game->flip();
    }

    delete (game);

    return 0;
}
