//
// Created by cpasjuste on 21/05/2021.
//

#include "game.h"
#include "ui.h"
#include "uitopplayers.h"
#include "uipausemenu.h"
#include "uimusic.h"

Ui::Ui(Game *game) : Rectangle(game->getSize()) {

    m_game = game;
    Vector2f scale = m_game->getScaling();

    font = new BMFont();
    font->loadFromFile(m_game->getIo()->getRomFsPath() + "textures/04b_19.fnt");
    font->setOffset({0, -(2 * scale.y)});

    fade = new RectangleShape(Ui::getSize());
    fade->setFillColor(Color::GrayLight);
    fade->setAlpha(0);
    fadeTween = new TweenAlpha(0, 230, 0.5f);
    fade->add(fadeTween);
    Ui::add(fade);

    scoreText = new Text("SCORE: 0", UI_FONT_SIZE, font);
    scoreText->setPosition(float((int) (4.0f * scale.x)), float((int) (4.0f * scale.y)));
    scoreText->setVisibility(Visibility::Hidden);
    scoreText->add(new TweenAlpha(0, 255, 3));
    Ui::add(scoreText);

    gameOverText = new Text("... GAME OVER ...", UI_FONT_SIZE, font);
    gameOverText->setOrigin(Origin::Center);
    gameOverText->setPosition(Ui::getSize().x / 2, float((int) (32.0f * scale.y)));
    gameOverText->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    gameOverText->add(new TweenColor(gameOverText->getFillColor(), Color::Yellow, 1, TweenLoop::PingPong));
    gameOverText->setVisibility(Visibility::Hidden);
    Ui::add(gameOverText);

    topPlayers = new UiTopPlayers(m_game, this, {0, 0, 400 * scale.x, (64 * scale.y) * 3});
    topPlayers->setOrigin(Origin::Center);
    topPlayers->setPosition(Ui::getSize().x / 2, Ui::getSize().y / 1.70f);
    topPlayers->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    topPlayers->setVisibility(Visibility::Hidden);
    Ui::add(topPlayers);

    pauseMenu = new UiPauseMenu(this);
    pauseMenu->setVisibility(Visibility::Hidden);
    add(pauseMenu);

    splashKyuhenTex = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/splashscreen-kyuhen.png");
    splashKyuhenTex->setScale(scale);
    splashKyuhenTex->add(new TweenAlpha(0, 255, 3));
    splashKyuhenTex->setVisibility(Visibility::Hidden);
    Ui::add(splashKyuhenTex);

    splashCpasTex = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/splashscreen-cpasjuste.png");
    splashCpasTex->setScale(scale);
    splashCpasTex->setOrigin(Origin::Center);
    splashCpasTex->setPosition(Ui::getSize().x / 2, Ui::getSize().y / 2);
    splashCpasTex->add(new TweenAlpha(0, 255, 3));
    splashCpasTex->setVisibility(Visibility::Hidden);
    Ui::add(splashCpasTex);

    title = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/title.png");
    title->setScale(scale);
    title->setOrigin(Origin::Center);
    title->setPosition(Ui::getSize().x / 2, (Ui::getSize().y / 2) - (64 * scale.y));
    title->add(new TweenAlpha(0, 255, 3));
    title->setVisibility(Visibility::Hidden);
    Ui::add(title);

#ifdef __SWITCH__
    pressStartText = new Text("PRESS (A) TO START", UI_FONT_SIZE, font);
#else
    pressStartText = new Text("PRESS (X) TO START", UI_FONT_SIZE, font);
#endif
    pressStartText->setOrigin(Origin::Center);
    pressStartText->setPosition(float((int) (Ui::getSize().x / 2)), float((int) (Ui::getSize().y - (128 * scale.y))));
    pressStartText->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    pressStartText->add(new TweenAlpha(0, 255, 2, TweenLoop::PingPong));
    pressStartText->setVisibility(Visibility::Hidden);
    Ui::add(pressStartText);

    bonusText = new Text("+10", UI_FONT_SIZE, font);
    bonusText->setOrigin(Origin::Center);
    bonusText->add(new TweenScale({0, 0}, {1, 1}, 0.25f));
    bonusText->setVisibility(Visibility::Hidden, false);
    Ui::add(bonusText);

    uiMusic = new UiMusic({Ui::getSize().x - 4, Ui::getSize().y - 4, 310, 18 * 2 + 8},
                          m_game->getColor(Game::SillyColor::green), m_game->getColor(Game::SillyColor::pink), font);
    uiMusic->setOrigin(Origin::BottomRight);
    Ui::add(uiMusic);

    m_game->getInput()->setRepeatDelay(10000);
}

void Ui::start() {
    splashClock.restart();
}

void Ui::setScore(long score) {
    scoreText->setString("SCORE: " + std::to_string(score));
}

void Ui::showGameOver() {
    m_game->getWarningShape()->setVisibility(Visibility::Hidden, true);
    fade->setVisibility(Visibility::Visible, true);
    gameOverText->setVisibility(Visibility::Visible, true);
    topPlayers->setVisibility(Visibility::Visible, true);
    pressStartText->setVisibility(Visibility::Visible, true);
    scoreText->setVisibility(Visibility::Hidden, true);
    pressStartText->setPosition(float((int) (Ui::getSize().x / 2)),
                                float((int) (Ui::getSize().y - (64 * m_game->getScaling().y))));
    m_game->getInput()->setRepeatDelay(10000);
    // scores
    Score myScore = m_game->getLeaderboard()->addScore(m_game->getScore());
    std::vector<Score> topScores = m_game->getLeaderboard()->getTopThree();
    topPlayers->setTopScores(topScores, myScore);
}

void Ui::hideGameOver() {
    if (gameOverText->isVisible()) {
        m_game->getWarningShape()->setVisibility(Visibility::Hidden, true);
        fade->setVisibility(Visibility::Hidden, true);
        gameOverText->setVisibility(Visibility::Hidden, true);
        topPlayers->setVisibility(Visibility::Hidden, true);
        scoreText->setVisibility(Visibility::Visible, true);
        pressStartText->setVisibility(Visibility::Hidden, true);
        m_game->getInput()->setRepeatDelay(0);
    }
}

void Ui::showBonusText(const Vector2f &pos, const std::string &text) {
    bonusText->setPosition(pos);
    bonusText->setVisibility(Visibility::Visible, true);
    bonusTextClock.restart();
}

void Ui::showMusicPlayer(const std::string &name, const std::string &author) {
    uiMusic->show(name, author);
}

void Ui::pause() {
    fade->setVisibility(Visibility::Visible, true);
    pauseMenu->setVisibility(Visibility::Visible, true);
    m_game->pause();
    m_game->getInput()->setRepeatDelay(10000);
}

void Ui::resume() {
    fade->setVisibility(Visibility::Hidden, true);
    pauseMenu->setVisibility(Visibility::Hidden, true);
    m_game->resume();
    m_game->getInput()->setRepeatDelay(0);
    m_game->getInput()->clear();
}

void Ui::onUpdate() {
    if (!gameStarted) {
        float elapsed = splashClock.getElapsedTime().asSeconds();
        if (elapsed > 14) {
            m_game->getMusic()->play(0);
            m_game->getGameView()->setVisibility(c2d::Visibility::Visible, true);
            title->setVisibility(Visibility::Visible, true);
            pressStartText->setVisibility(Visibility::Visible, true);
            gameStarted = true;
        } else if (elapsed > 11 && elapsed <= 14 && splashCpasTex->isVisible()) {
            splashCpasTex->setVisibility(Visibility::Hidden, true);
        } else if (elapsed > 8 && elapsed <= 11 && !splashCpasTex->isVisible()) {
            splashCpasTex->setVisibility(Visibility::Visible, true);
        } else if (elapsed > 5 && elapsed <= 8 && splashKyuhenTex->isVisible()) {
            splashKyuhenTex->setVisibility(Visibility::Hidden, true);
        } else if (elapsed > 2 && elapsed <= 5 && !splashKyuhenTex->isVisible()) {
            splashKyuhenTex->setVisibility(Visibility::Visible, true);
        }
    } else {
        if (bonusText->isVisible() && bonusTextClock.getElapsedTime().asSeconds() > 2) {
            bonusText->setVisibility(Visibility::Hidden, true);
        }
    }

    Rectangle::onUpdate();
}

bool Ui::onInput(Input::Player *players) {
    unsigned int keys = players[0].keys;
    if (!gameStarted) {
        if (keys & Input::Key::Start) {
            if (buttonPressCount > 0) {
                m_game->getMusic()->play(0);
                splashKyuhenTex->setVisibility(Visibility::Hidden, false);
                splashCpasTex->setVisibility(Visibility::Hidden, false);
                m_game->getGameView()->setVisibility(c2d::Visibility::Visible, true);
                title->setVisibility(Visibility::Visible, true);
                pressStartText->setVisibility(Visibility::Visible, true);
                gameStarted = true;
            }
            buttonPressCount++;
        }
    } else {
        if (keys & Input::Key::Fire1 && (title->isVisible() || m_game->isEnded())) {
            title->setVisibility(Visibility::Hidden, true);
            pressStartText->setVisibility(Visibility::Hidden, false);
            scoreText->setVisibility(Visibility::Visible, true);
            m_game->restart();
            m_game->getInput()->setRepeatDelay(0);
        } else if (keys & Input::Key::Fire5) {
            m_game->getMusic()->playPrev();
            if (!title->isVisible()) {
                m_game->delay(150);
            }
        } else if (keys & Input::Key::Fire6) {
            m_game->getMusic()->playNext();
            if (!title->isVisible()) {
                m_game->delay(150);
            }
        } else if (keys & Input::Key::Start && !m_game->isEnded() && !title->isVisible()) {
            if (m_game->isPaused()) {
                resume();
            } else if (!m_game->isPaused()) {
                pause();
            }
        }
    }

    return Rectangle::onInput(players);
}

Ui::~Ui() {
    delete (font);
}
