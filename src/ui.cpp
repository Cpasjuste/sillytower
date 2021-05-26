//
// Created by cpasjuste on 21/05/2021.
//

#include "ui.h"

Ui::Ui(Renderer *renderer) : Rectangle(renderer->getSize()) {

    font = new Font();
    font->loadFromFile(renderer->getIo()->getRomFsPath() + "font.ttf");
    font->setOffset({0, -6});

    scoreText = new Text("SCORE: 0", C2D_DEFAULT_CHAR_SIZE, font);
    scoreText->setPosition(4, 4);
    scoreText->setFillColor(Color::GrayLight);
    scoreText->setOutlineThickness(1);
    scoreText->setOutlineColor(Color::Black);
    Ui::add(scoreText);

    gameOverText = new Text("GAME OVER", 64, font);
    gameOverText->setOrigin(Origin::Top);
    gameOverText->setPosition(Ui::getSize().x / 2, 32);
    gameOverText->setFillColor(Color::GrayLight);
    gameOverText->setOutlineThickness(2);
    gameOverText->setOutlineColor(Color::Black);
    gameOverText->add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    gameOverText->setVisibility(Visibility::Hidden);
    Ui::add(gameOverText);
}

void Ui::setScore(int score) {
    if (scoreText) {
        scoreText->setString("SCORE: " + std::to_string(score));
    }
}

void Ui::showGameOver() {
    if (gameOverText) {
        gameOverText->setVisibility(Visibility::Visible, true);
    }
}

void Ui::hideGameOver() {
    if (gameOverText) {
        gameOverText->setVisibility(Visibility::Hidden, true);
    }
}

Ui::~Ui() {
    delete (font);
}

