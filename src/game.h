//
// Created by cpasjuste on 19/05/2021.
//

#ifndef SILLYTOWER_GAME_H
#define SILLYTOWER_GAME_H

#include <random>

#include "cross2d/c2d.h"
#include "ui.h"
#include "cube.h"
#include "youlead.h"
#include "music.h"
#include "bird.h"

#define CUBE_MIN_WIDTH 100
#define CUBE_MAX_WIDTH 300
#define CUBE_HEIGHT 100
#define CLOUD_MAX 30
#define STARS_MAX 100
#define STATIC_CUBE_MULTIPLIER 20
#define EXPLODING_CUBE_MULTIPLIER 10
#define BIRD_SPAWN_MULTIPLIER 10

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

    void pause();

    void resume();

    void restart();

    bool isPaused() const {
        return paused;
    };

    bool isEnded() const {
        return ended;
    }

    Cube *spawnCube(float y = 0);

    void removeCube();

    Cube *getCube() {
        return cube;
    }

    void setCube(Cube *c) {
        cube = c;
    }

    int getCubeMinWidth() const {
        return (int) ((float) CUBE_MIN_WIDTH * m_scaling.x);
    }

    int getCubeMaxWidth() const {
        return (int) ((float) CUBE_MAX_WIDTH * m_scaling.x);
    }

    int getCubeHeight() const {
        return (int) ((float) CUBE_HEIGHT * m_scaling.y);
    }

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

    MusicPlayer *getMusic() {
        return music;
    }

    void addToScore(int bonus) {
        score += bonus;
        ui->setScore(score - 1);
    }

    long getScore() const {
        return (long) score - 1;
    }

    Texture *getCloudSpriteSheet() {
        return cloudSpriteSheet;
    }

    Texture *getSmokeSpriteSheet() {
        return smokeSpriteSheet;
    }

    RectangleShape *getWarningShape() {
        return warningShape;
    }

    TweenAlpha *getWarningTween() {
        return warningTween;
    }

    Color getColor(const SillyColor &color) const {
        return colors[color];
    };

    Color getColorRandom() const {
        return colors[Utility::random(0, 4)];
    }

    Vector2f getScaling() const {
        return m_scaling;
    }

    void quit() {
        m_quit = true;
    }

    bool shouldQuit() {
        return m_quit;
    }

private:

    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    Vector2f m_scaling = {1, 1};

    //
    Ui *ui;
    PhysicsWorld *world;
    YouLead *leaderboard;
    Rectangle *gameView;
    MusicPlayer *music;

    // sprites
    Texture *cloudSpriteSheet = nullptr;
    Texture *birdSpriteSheet;
    Texture *smokeSpriteSheet;

    Bird *bird = nullptr;
    b2Body *floor = nullptr;
    b2Body *firstCube = nullptr;
    b2Body *secondCube = nullptr;
    Cube *cube = nullptr;
    std::vector<Cube *> cubes;
    bool needSpawn = false;
    int score = 0;
    int staticMultiplier = Utility::random(STATIC_CUBE_MULTIPLIER - 5, STATIC_CUBE_MULTIPLIER + 5);
    int explodingMultiplier = Utility::random(EXPLODING_CUBE_MULTIPLIER - 5, EXPLODING_CUBE_MULTIPLIER + 5);
    int birdMultiplier = Utility::random(BIRD_SPAWN_MULTIPLIER - 5, BIRD_SPAWN_MULTIPLIER + 5);
    bool paused = true;
    bool ended = false;
    bool m_quit = false;

    // camera
    TweenScale *cameraScaleTween;

    // cube rotation warning (fade)
    RectangleShape *warningShape;
    TweenAlpha *warningTween;

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
