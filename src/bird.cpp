//
// Created by cpasjuste on 04/06/2021.
//

#include "cross2d/c2d.h"
#include "game.h"
#include "bird.h"

using namespace c2d;

Bird::Bird(Game *game, Texture *texture)
        : AnimatedSprite(texture, {128, 128}, 4, 8, 1.5f) {

    m_game = game;

    // position (x)
    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float extend = (width / scale) - width;
    float x = -((extend / 2) + Bird::getSize().x);
    // position (y)
    float screenHeight = m_game->getSize().y / m_game->getGameView()->getScale().y;
    m_minY = Utility::random(screenHeight * 0.5f, screenHeight * 0.7f);
    // set bird pos
    Bird::setPosition(x - 200, m_minY);
    Bird::addPhysicsBody(m_game->getPhysicsWorld(), b2_dynamicBody);

    // disable collisions
    b2Filter filter = Bird::getPhysicsBodyFixture()->GetFilterData();
    filter.maskBits = 2;
    Bird::getPhysicsBodyFixture()->SetFilterData(filter);

    getPhysicsBody()->ApplyForce({400, 500}, getPhysicsBody()->GetWorldCenter(), true);
}

void Bird::onUpdate() {

    float width = m_game->getGameView()->getSize().x;
    float scale = m_game->getGameView()->getScale().x;
    float screenRight = (((width / scale) - width) / 2) + width;
    if (getPosition().x > screenRight + (getSize().x * getScale().x)) {
        removePhysicsBody();
        setVisibility(Visibility::Hidden);
        return;
    }

    if (!isVisible() || !getPhysicsBody()) {
        return;
    }

    Cube *cube = m_game->getCube();
    if (cube && cube->getGlobalBounds().intersects(getGlobalBounds())) {
        // remove cube
        m_game->removeCube(m_game->getCube());
        Cube *c = m_game->spawnCube();
        m_game->setCube(c);
        removePhysicsBody();
        setVisibility(Visibility::Hidden);
        // show ui bonus text
        float scaling = m_game->getGameView()->getScale().x;
        float worldWidth = m_game->getGameView()->getSize().x / scaling;
        float worldExtend = (worldWidth - m_game->getGameView()->getSize().x) / 2;
        float birdHalfWidth = (getSize().x * scaling) / 2;
        float birdHalfHeight = (getSize().y * scaling) / 2;
        Vector2f screenPos = {((worldExtend + getPosition().x) * scaling) + birdHalfWidth,
                              (m_game->getSize().y - (getPosition().y * scaling)) - birdHalfHeight};
        m_game->getUi()->showBonusText(screenPos, "+10");
        m_game->addToScore(10);
        return;
    }

    if (getPhysicsBody()->GetLinearVelocity().x < 20) {
        getPhysicsBody()->ApplyForceToCenter({50, 0}, true);
    }

    if (getPosition().y < m_minY) {
        getPhysicsBody()->ApplyForceToCenter({0, 500}, true);
    }

    AnimatedSprite::onUpdate();
}
