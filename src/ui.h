//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_UI_H
#define SILLYTOWER_UI_H

#include "cross2d/c2d.h"

using namespace c2d;

class Ui : public Rectangle {

public:
    explicit Ui(Renderer *renderer);

    ~Ui() override;

    void setScore(int score);

    void showGameOver();

private:
    Font *font = nullptr;
    Text *scoreText = nullptr;
    Text *gameOverText = nullptr;
};

#endif //SILLYTOWER_UI_H
