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
    Cloud::setPosition(game->getSize().x + Cloud::getSize().x, game->getSize().y / 2);

    /*
    tweenPosition = new TweenPosition({game->getSize().x + 130, game->getSize().y / 2},
                                      {-130, game->getSize().y / 2},
                                      40.0f);
    Cloud::add(tweenPosition);
    tweenPosition->play();
    */
}

void Cloud::onUpdate() {

    move(-1, 0);

    // TODO
    printf("%f\n", getPosition().x);
    if (getPosition().x < 0) {
        printf("%f\n", getPosition().x);
    }

    Sprite::onUpdate();
}
