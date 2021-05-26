//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include <random>

#include "ui.h"
#include "cube.h"
#include "leaderboard/youlead.h"

#define CUBE_MIN_WIDTH 100
#define CUBE_MAX_WIDTH 300
#define CUBE_MIN_HEIGHT 100
#define CUBE_MAX_HEIGHT 100

class Game : public C2DRenderer, b2ContactListener {

public:

    explicit Game(const Vector2f &size);

    ~Game() override;

    Cube *spawnCube(float y = 0);

    void spawnCloud();

    void BeginContact(b2Contact *contact) override;

    Rectangle *getGameView() {
        return gameView;
    }

    Texture *getSpriteSheet() {
        return spriteSheet;
    }

private:

    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    //
    Ui *ui;
    PhysicsWorld *world;
    YouLead *leaderboard;
    Rectangle *gameView;

    // sprites
    Texture *spriteSheet = nullptr;

    // random
    std::mt19937 mt;
    std::uniform_real_distribution<float> cube_x;
    std::uniform_real_distribution<float> cube_width;
    std::uniform_real_distribution<float> cube_height;
    std::uniform_real_distribution<float> cube_color;

    b2Body *floor = nullptr;
    b2Body *firstCube = nullptr;
    b2Body *secondCube = nullptr;
    Cube *cube = nullptr;
    std::vector<Cube *> cubes;
    int cubeCount = 0;
    bool needSpawn = false;

    // camera
    TweenScale *cameraScaleTween = nullptr;
};

#endif //SILLYTOWER_GAME_H
