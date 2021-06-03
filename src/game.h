//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include "cross2d/c2d.h"
#include "ui.h"
#include "cube.h"
#include "youlead.h"
#include "music.h"
#include "background.h"

#define CUBE_MIN_WIDTH 100
#define CUBE_MAX_WIDTH 300
#define CUBE_MIN_HEIGHT 100
#define CUBE_MAX_HEIGHT 100
#define CLOUD_MAX 30
#define STARS_MAX 100
#define STATIC_CUBE_MULTIPLIER 20
#define EXPLODING_CUBE_MULTIPLIER 10
#define CAMERA_MIN_SCALE 0.3f

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

    Background *getBackground() {
        return background;
    }

    Ui *getUi() {
        return ui;
    }

    YouLead *getLeaderboard() {
        return leaderboard;
    }

    MusicPlayer *getMusic() {
        return music;
    }

    long getScore() const {
        return (long) spawnedCubes - 1;
    }

    Texture *getSpriteSheet() {
        return spriteSheet;
    }

    Color getColor(const SillyColor &color) const {
        return colors[color];
    };

    Color getColorRandom() const {
        return colors[Utility::random(0, 4)];
    }

private:

    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    //
    Ui *ui;
    PhysicsWorld *world;
    YouLead *leaderboard;
    Rectangle *gameView;
    Background *background;
    MusicPlayer *music;

    // sprites
    Texture *spriteSheet = nullptr;

    b2Body *floor = nullptr;
    b2Body *firstCube = nullptr;
    b2Body *secondCube = nullptr;
    Cube *cube = nullptr;
    std::vector<Cube *> cubes;
    bool needSpawn = false;
    int spawnedCubes = 0;
    int staticMultiplier = Utility::random(STATIC_CUBE_MULTIPLIER - 5, STATIC_CUBE_MULTIPLIER + 5);
    int explodingMultiplier = Utility::random(EXPLODING_CUBE_MULTIPLIER - 5, EXPLODING_CUBE_MULTIPLIER + 5);

    // camera
    TweenScale *cameraScaleTween;
    TweenPosition *cameraPosTween;

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
