//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_UI_H
#define SILLYTOWER_UI_H

#ifdef __3DS__
#define UI_FONT_SIZE 14
#else
#define UI_FONT_SIZE C2D_DEFAULT_CHAR_SIZE
#endif

using namespace c2d;

class Game;

class UiTopPlayers;

class UiPauseMenu;

class UiMusic;

class Ui : public Rectangle {

public:
    explicit Ui(Game *game);

    ~Ui() override;

    void start();

    void setScore(long score);

    void showGameOver();

    void hideGameOver();

    void showBonusText(const Vector2f &pos, const std::string &text);

    void showMusicPlayer(const std::string &name, const std::string &author);

    void pause();

    void resume();

    Game *getGame() {
        return m_game;
    }

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
    Text *pressStartText;
    Text *bonusText;
    C2DClock bonusTextClock;
    Texture *title;
    RectangleShape *fade;
    TweenAlpha *fadeTween;
    bool gameStarted = false;
    int buttonPressCount = 0;
    // pause menu
    UiPauseMenu *pauseMenu;
    // top players
    UiTopPlayers *topPlayers;
    // music ui
    UiMusic *uiMusic;
    C2DClock inputClock;

    // splashes
    C2DClock splashClock;
    Texture *splashKyuhenTex;
    Texture *splashCpasTex;
};

#endif //SILLYTOWER_UI_H
