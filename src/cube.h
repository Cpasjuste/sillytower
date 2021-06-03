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
    enum Mode {
        Normal,
        Static,
        Exploding
    };

    explicit Cube(Game *game, const FloatRect &rect);

    void playTween(const Color &toColor);

    void stopTween(const Color &toColor);

    void setMode(const Mode &mode) {
        m_mode = mode;
    }

    Mode getMode() const {
        return m_mode;
    }

private:
    void onUpdate() override;

    TweenColor *m_tweenColor = nullptr;
    Mode m_mode = Normal;
};

#endif //SILLYTOWER_CUBE_H
