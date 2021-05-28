//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include <random>

#include "ui.h"
#include "cube.h"
#include "youlead.h"

#define CUBE_MIN_WIDTH 100
#define CUBE_MAX_WIDTH 300
#define CUBE_MIN_HEIGHT 100
#define CUBE_MAX_HEIGHT 100
#define CLOUD_MAX 20
#define STATIC_CUBE_MULTIPLIER 20

class Game : public C2DRenderer, b2ContactListener {

public:

    enum SillyColor {
        magenta,
        blue,
        green,
        yellow,
        pink
    };

    explicit Game(const Vector2f &size);

    ~Game() override;

    void start();

    Cube *spawnCube(float y = 0);

    void BeginContact(b2Contact *contact) override;

    PhysicsWorld *getPhysicsWorld() {
        return world;
    }

    Rectangle *getGameView() {
        return gameView;
    }

    Ui *getUi() {
        return ui;
    }

    YouLead *getLeaderboard() {
        return leaderboard;
    }

    long getScore() const {
        return cubeCount;
    }

    Texture *getSpriteSheet() {
        return spriteSheet;
    }

    Color getColor(const SillyColor &color) const {
        return colors[color];
    };

    Color getColorRandom() const {
        return colors[rand() % 5];
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

    b2Body *floor = nullptr;
    b2Body *firstCube = nullptr;
    b2Body *secondCube = nullptr;
    Cube *cube = nullptr;
    std::vector<Cube *> cubes;
    int cubeCount = 0;
    bool needSpawn = false;

    // camera
    TweenScale *cameraScaleTween;

    // colors
    std::vector<Color> colors = {
            {234, 159, 230},
            {121, 133, 224},
            {118, 206, 134},
            {245, 223, 141},
            {242, 148, 150}
    };
};

#endif //SILLYTOWER_GAME_H
