//
// Created by cpasjuste on 04/06/2021.
//

#ifndef SILLYTOWER_BIRD_H
#define SILLYTOWER_BIRD_H

class Bird : public c2d::AnimatedSprite {
public:
    Bird(Game *game, c2d::Texture *texture);

private:
    void onUpdate() override;

    Game *m_game;
    float m_minY;
};

#endif //SILLYTOWER_BIRD_H
