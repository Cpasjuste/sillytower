//
// Created by cpasjuste on 21/05/2021.
//

#include "ui.h"

Ui::Ui(Renderer *renderer) : Rectangle(renderer->getSize()) {

    font = new Font();
    font->loadFromFile(renderer->getIo()->getRomFsPath() + "font.ttf");
    font->setOffset({0, -4});

    scoreText = new Text("SCORE: 0", C2D_DEFAULT_CHAR_SIZE, font);
    scoreText->setPosition(8, 8);
    scoreText->setFillColor(Color::Yellow);
    scoreText->setOutlineThickness(2);
    scoreText->setOutlineColor(Color::Black);
    Ui::add(scoreText);

    gameOverText = new Text("GAME OVER", 64, font);
    gameOverText->setOrigin(Origin::Center);
    gameOverText->setPosition(Ui::getSize().x / 2, Ui::getSize().y / 2);
    gameOverText->setFillColor(Color::Yellow);
    gameOverText->setOutlineThickness(3);
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

Ui::~Ui() {
    delete (font);
}
