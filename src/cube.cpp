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

void Cube::playTween() {
    if (!tweenColor) {
        tweenColor = new TweenColor(getFillColor(), Color::GrayLight, 0.5f, TweenLoop::PingPong);
        Cube::add(tweenColor);
    }
    tweenColor->play();
}

void Cube::stopTween() {
    if (tweenColor) {
        tweenColor->setState(TweenState::Stopped);
        setFillColor(Color::GrayLight);
    }
}

void Cube::onUpdate() {

    if (getRotation() > 15) {
        //printf("WARNING: %f\n", getRotation());
    }

    RectangleShape::onUpdate();
}
