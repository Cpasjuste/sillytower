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
    setTextureRect(sillytower::Utility::getTextureRect(getTexture(), 4, 0, 0));
    Cloud::setOrigin(Origin::Center);

    float x = ((float) rand() / (float) (RAND_MAX)) * m_game->getSize().x;
    float y = ((float) rand() / (float) (RAND_MAX)) * (m_game->getSize().y / 2);
    Cloud::setPosition(x, y);
}

void Cloud::reset() {
    // speed 0.1f <> 0.5f
    m_speed = 0.1f + (float) rand() / ((float) RAND_MAX / (0.5f - 0.1f));

    //float screenTop = getSize().y / gameView->getScale().y;
    float x = (m_game->getGameView()->getSize().x * m_game->getGameView()->getScale().x) * -1;
            // (-(m_game->getGameView()->getSize().x / 2) / m_game->getGameView()->getScale().x) + 128;

    float y = ((float) rand() / (float) (RAND_MAX)) *
              ((m_game->getGameView()->getSize().y / 2) * m_game->getGameView()->getScale().x);
    Cloud::setPosition(x, y);

    printf("Cloud::reset: speed: %f, x: %f, y: %f\n", m_speed, x, y);
}

void Cloud::onUpdate() {

    move(5, 0);


    if (getPosition().x > m_game->getGameView()->getSize().x * m_game->getGameView()->getScale().x) {
        reset();
    }

    Sprite::onUpdate();
}

