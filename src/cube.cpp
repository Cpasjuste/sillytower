//
// Created by cpasjuste on 20/05/2021.
//

#include "game.h"
#include "cube.h"

Cube::Cube(Game *game, const FloatRect &rect) : RectangleShape(rect) {
    Cube::setFillColor(game->getColorRandom());
    Cube::setOutlineColor(Color::Black);
    Cube::setOutlineThickness(2);
    Cube::addPhysicsBody(game->getPhysicsWorld());
}

void Cube::playTween(const Color &toColor) {
    if (!m_tweenColor) {
        m_tweenColor = new TweenColor(getFillColor(), toColor, 0.5f, TweenLoop::PingPong);
        Cube::add(m_tweenColor);
    }
    m_tweenColor->play();
}

void Cube::stopTween(const Color &toColor) {
    if (m_tweenColor) {
        m_tweenColor->setState(TweenState::Stopped);
        setFillColor(toColor);
    }
}

void Cube::onUpdate() {

    if (getRotation() > 15 && getRotation() < 345) {
        //printf("WARNING: %f\n", getRotation());
    }

    RectangleShape::onUpdate();
}
