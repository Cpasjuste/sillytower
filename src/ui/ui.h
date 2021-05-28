//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_UI_H
#define SILLYTOWER_UI_H

#include "cross2d/c2d.h"

using namespace c2d;

class Game;

class UiTopPlayers;

class Ui : public Rectangle {

public:
    explicit Ui(Game *game);

    ~Ui() override;

    void start();

    void setScore(int score);

    void showGameOver();

    void hideGameOver();

    BMFont *getFont() {
        return font;
    }

private:
    void onUpdate() override;

    bool onInput(Input::Player *players) override;

    Game *m_game;
    BMFont *font;
    Text *scoreText;
    Text *gameOverText;
    Text *pressStart;
    Texture *title;
    RectangleShape *fade;
    TweenAlpha *fadeTween;
    bool gameStarted = false;
    int buttonPressCount = 0;

    // top players
    UiTopPlayers *topPlayers;

    // splashes
    C2DClock splashClock;
    Texture *splashKyuhenTex;
    Texture *splashCpasTex;
};

#endif //SILLYTOWER_UI_H
