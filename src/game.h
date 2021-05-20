//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include <random>
#include "cross2d/c2d.h"
#include "cube.h"

#define CUBE_MIN_WIDTH 300
#define CUBE_MAX_WIDTH 600
#define CUBE_MIN_HEIGHT 100
#define CUBE_MAX_HEIGHT 120

using namespace c2d;

class Game : public C2DRenderer, b2ContactListener {

public:

    Game();

    ~Game() override;

    Cube *spawnCube();

    void BeginContact(b2Contact *contact) override;

private:

    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    // random
    std::mt19937 mt;
    std::uniform_real_distribution<float> cube_x;
    std::uniform_real_distribution<float> cube_width;
    std::uniform_real_distribution<float> cube_height;
    std::uniform_real_distribution<float> cube_color;

    b2Body *floor = nullptr;
    b2Body *firstCube = nullptr;
    Cube *cube = nullptr;
    int cubeCount = 0;
    bool needSpawn = false;

    // camera
    TweenScale *cameraScaleTween = nullptr;
    float cameraScaleFactor = 1;
};

#endif //SILLYTOWER_GAME_H
