//
// Created by cpasjuste on 26/05/2021.
//

#include "cross2d/c2d.h"
#include "game.h"
#include "cloud.h"
#include "utility.h"

Cloud::Cloud(Game *game) : Sprite() {
    m_game = game;
    setTexture(m_game->getCloudSpriteSheet());
    setTextureRect(st::Utility::getTextureRect(getTexture(), 4, Utility::random(0, 4), 0));
    float scaling = Utility::random(2.0f, 4.0f);
    Cloud::setScale(scaling, scaling);
    Cloud::setOrigin(Origin::Center);
    reset();
}

void Cloud::reset() {
    // speed 0.1f <> 0.5f
    m_speed = Utility::random(1.0f, 3.0f);
    // position (x)
    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float extend = (width / scale) - width;
    float x = -((extend / 2) + getSize().x);
    // position (y)
    float height = m_game->getGameView()->getSize().y;
    float min = -(height * 2.5f);
    float max = min - (height * 2.5f);
    float y = Utility::random(min, max);
    //printf("min: %f, max: %f, y: %f\n", min, max, y);

    setPosition(x, y);
    setTextureRect(st::Utility::getTextureRect(getTexture(), 4, Utility::random(0, 4), 0));
}

void Cloud::onUpdate() {

    move(m_speed, 0);

    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float screenRight = (((width / scale) - width) / 2) + width;
    if (getPosition().x > screenRight + (getSize().x * getScale().x)) {
        reset();
    }

    Sprite::onUpdate();
}
