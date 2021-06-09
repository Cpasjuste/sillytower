//
// Created by cpasjuste on 08/12/16.
//

#include "cross2d/c2d.h"
#include "game.h"

using namespace c2d;

#ifdef __PSP2__

#include <psp2/power.h>

int _newlib_heap_size_user = 192 * 1024 * 1024;
#endif

int main(int argc, char *argv[]) {

#ifdef __PSP2__
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
#endif

    auto *game = new Game({960, 544});

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
