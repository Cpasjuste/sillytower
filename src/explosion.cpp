//
// Created by cpasjuste on 04/06/2021.
//

#include "cross2d/c2d.h"
#include "explosion.h"

using namespace c2d;

Explosion::Explosion(c2d::Texture *texture, const Vector2f &pos)
        : AnimatedSprite(texture, {128, 128}, 4, 8, 0.5f) {

    Explosion::setScale(2, 2);
    Explosion::setOrigin(Origin::Center);
    Explosion::setPosition(pos);
    m_clock.restart();
}

void Explosion::onUpdate() {
    if (m_clock.getElapsedTime().asSeconds() > getDuration()) {
        delete (this);
        return;
    }
    AnimatedSprite::onUpdate();
}
