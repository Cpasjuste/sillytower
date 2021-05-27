//
// Created by cpasjuste on 26/05/2021.
//

#ifndef SILLYTOWER_CLOUD_H
#define SILLYTOWER_CLOUD_H

using namespace c2d;

class Cloud : public Sprite {

public:
    explicit Cloud(Game *game);

    void reset();

private:
    void onUpdate() override;

    Game *m_game = nullptr;
    float m_speed = 0.1f;
};

#endif //SILLYTOWER_CLOUD_H
