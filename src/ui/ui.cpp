//
// Created by cpasjuste on 21/05/2021.
//

#include "game.h"
#include "ui.h"
#include "uitopplayers.h"
#include "uimusic.h"

Ui::Ui(Game *game) : Rectangle(game->getSize()) {

    m_game = game;

    font = new BMFont();
    font->loadFromFile(m_game->getIo()->getRomFsPath() + "textures/04b_19.fnt");
    font->setOffset({0, -2});

    fade = new RectangleShape(Ui::getSize());
    fade->setFillColor(Color::GrayLight);
    fade->setAlpha(0);
    fadeTween = new TweenAlpha(0, 230, 1);
    fade->add(fadeTween);
    Ui::add(fade);

    scoreText = new Text("SCORE: 0", C2D_DEFAULT_CHAR_SIZE, font);
    scoreText->setPosition(4, 4);
    scoreText->setVisibility(Visibility::Hidden);
    scoreText->add(new TweenAlpha(0, 255, 3));
    Ui::add(scoreText);

    gameOverText = new Text("... GAME OVER ...", C2D_DEFAULT_CHAR_SIZE, font);
    gameOverText->setOrigin(Origin::Center);
    gameOverText->setPosition(Ui::getSize().x / 2, 32 * m_game->getScaling().y);
    gameOverText->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    gameOverText->add(new TweenColor(gameOverText->getFillColor(), Color::Yellow, 1, TweenLoop::PingPong));
    gameOverText->setVisibility(Visibility::Hidden);
    Ui::add(gameOverText);

    topPlayers = new UiTopPlayers(m_game, this, {0, 0, 400, 64 * 3});
    topPlayers->setOrigin(Origin::Center);
    topPlayers->setPosition(Ui::getSize().x / 2, Ui::getSize().y / 1.70f);
    topPlayers->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    topPlayers->setVisibility(Visibility::Hidden);
    Ui::add(topPlayers);

    splashKyuhenTex = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/splashscreen-kyuhen.png");
    splashKyuhenTex->setScale(m_game->getScaling());
    splashKyuhenTex->add(new TweenAlpha(0, 255, 3));
    splashKyuhenTex->setVisibility(Visibility::Hidden);
    Ui::add(splashKyuhenTex);

    splashCpasTex = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/splashscreen-cpasjuste.png");
    splashCpasTex->setScale(m_game->getScaling());
    splashCpasTex->setOrigin(Origin::Center);
    splashCpasTex->setPosition(Ui::getSize().x / 2, Ui::getSize().y / 2);
    splashCpasTex->add(new TweenAlpha(0, 255, 3));
    splashCpasTex->setVisibility(Visibility::Hidden);
    Ui::add(splashCpasTex);

    title = new C2DTexture(m_game->getIo()->getRomFsPath() + "textures/title.png");
    title->setOrigin(Origin::Center);
    title->setPosition(Ui::getSize().x / 2, (Ui::getSize().y / 2) - 64);
    title->add(new TweenAlpha(0, 255, 3));
    title->setVisibility(Visibility::Hidden);
    Ui::add(title);

    pressStartText = new Text("PRESS START", C2D_DEFAULT_CHAR_SIZE, font);
    pressStartText->setOrigin(Origin::Center);
    pressStartText->setPosition(Ui::getSize().x / 2, Ui::getSize().y - (128 * m_game->getScaling().y));
    pressStartText->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    pressStartText->add(new TweenAlpha(0, 255, 2, TweenLoop::PingPong));
    pressStartText->setVisibility(Visibility::Hidden);
    Ui::add(pressStartText);

    bonusText = new Text("+10", C2D_DEFAULT_CHAR_SIZE, font);
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
    pressStartText->setPosition(Ui::getSize().x / 2, Ui::getSize().y - (64 * m_game->getScaling().y));
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
    if (!gameStarted) {
        if (players[0].keys & Input::Key::Start) {
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
    } else if (players[0].keys & Input::Key::Start && title->isVisible()) {
        title->setVisibility(Visibility::Hidden, true);
        pressStartText->setVisibility(Visibility::Hidden, false);
        scoreText->setVisibility(Visibility::Visible, true);
        m_game->start();
        m_game->getInput()->setRepeatDelay(0);
    } else if (players[0].keys & Input::Key::Fire5) {
        m_game->getMusic()->playPrev();
        if (!title->isVisible()) {
            m_game->delay(150);
        }
    } else if (players[0].keys & Input::Key::Fire6) {
        m_game->getMusic()->playNext();
        if (!title->isVisible()) {
            m_game->delay(150);
        }
    }

    return Rectangle::onInput(players);
}

Ui::~Ui() {
    delete (font);
}

