//
// Created by cpasjuste on 08/12/16.
//

#include "main.h"

Game *game;
Ui *ui;
YouLead *leaderboard;

int main(int argc, char *argv[]) {

    auto renderer = new C2DRenderer({C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT});

    leaderboard = new YouLead();
    ui = new Ui(renderer);
    game = new Game(renderer);
    renderer->add(game);
    renderer->add(ui);

    while (true) {

        unsigned int keys = renderer->getInput()->getKeys();
        if (keys & EV_QUIT) {
            break;
        }

        renderer->flip();
    }

    delete (leaderboard);
    delete (renderer);

    return 0;
}
