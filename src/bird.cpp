//
// Created by cpasjuste on 04/06/2021.
//

#include "cross2d/c2d.h"
#include "game.h"

using namespace c2d;

Bird::Bird(Game *game, Texture *texture) : AnimatedSprite(texture, {128, 128}, 4, 8, 1.5f) {

    m_game = game;

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

    TODO
    Bird::setPosition(x, y);
    Bird::addPhysicsBody(m_game->getPhysicsWorld(), b2_dynamicBody, )
}
