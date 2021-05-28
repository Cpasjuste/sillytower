//
// Created by cpasjuste on 20/05/2021.
//

#ifndef SILLYTOWER_CUBE_H
#define SILLYTOWER_CUBE_H

#include "cross2d/c2d.h"

using namespace c2d;

class Game;

class Cube : public RectangleShape {

public:
    explicit Cube(Game *game, const FloatRect &rect);

    void playTween();

    void stopTween();

private:
    void onUpdate() override;

    TweenColor *tweenColor = nullptr;
};

#endif //SILLYTOWER_CUBE_H
