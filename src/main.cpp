//
// Created by cpasjuste on 08/12/16.
//

#include <leaderboard/youlead.h>
#include "main.h"

Game *game;
Ui *ui;

int main(int argc, char *argv[]) {

    auto lb = new YouLead();
    //lb->getScore();
    lb->getScores();
    delete (lb);

#if 0
    auto renderer = new C2DRenderer({C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT});

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

    delete (renderer);
#endif

    return 0;
}
