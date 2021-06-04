//
// Created by cpasjuste on 04/06/2021.
//

#ifndef SILLYTOWER_EXPLOSION_H
#define SILLYTOWER_EXPLOSION_H

class Explosion : public c2d::AnimatedSprite {
public:
    Explosion(c2d::Texture *texture, const c2d::Vector2f &pos);

private:
    void onUpdate() override;

    c2d::C2DClock m_clock;
};

#endif //SILLYTOWER_EXPLOSION_H
