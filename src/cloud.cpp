//
// Created by cpasjuste on 26/05/2021.
//

#include "cross2d/c2d.h"
#include "game.h"
#include "cloud.h"
#include "utility.h"

Cloud::Cloud(Game *game) : Sprite() {
    m_game = game;
    setTexture(m_game->getSpriteSheet());
    setTextureRect(st::Utility::getTextureRect(getTexture(), 4, rand() % 4, 0));
    Cloud::setScale(2, 2);
    Cloud::setOrigin(Origin::Center);
    reset();
}

void Cloud::reset() {
    // speed 0.1f <> 0.5f
    m_speed = 1.0f + (float) rand() / ((float) RAND_MAX / (3.0f - 1.0f));

    // position (x)
    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float extend = (width / scale) - width;
    float x = -((extend / 2) + getSize().x);
    // position (y)
    float height = m_game->getGameView()->getSize().y;
    float min = -(height * 3);
    float max = min - (height * 3);
    float y = min + (float) rand() / ((float) RAND_MAX / (max - min));
    //printf("min: %f, max: %f, y: %f\n", min, max, y);

    Cloud::setPosition(x, y);
    setTextureRect(st::Utility::getTextureRect(getTexture(), 4, rand() % 4, 0));
}

void Cloud::onUpdate() {

    move(m_speed, 0);

    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float screenRight = (((width / scale) - width) / 2) + width;
    if (getPosition().x > screenRight + getSize().x) {
        reset();
    }

    Sprite::onUpdate();
}
