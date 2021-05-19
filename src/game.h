//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include "cross2d/c2d.h"

using namespace c2d;

class Game : public C2DRenderer, b2ContactListener {

public:

    Game();

    ~Game() override;

    b2Body *spawnCube();

    void BeginContact(b2Contact *contact) override;

private:

    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    C2DTexture *cubeTextureSheet = nullptr;
    b2Body *cube = nullptr;

    //C2DClock spawnTimer;
    //float spawnDelay = 5;
    bool needSpawn = false;
};

#endif //SILLYTOWER_GAME_H
