//
// Created by cpasjuste on 26/05/2021.
//

#ifndef SILLYTOWER_CLOUD_H
#define SILLYTOWER_CLOUD_H

using namespace c2d;

class Cloud : public Sprite {

public:
    explicit Cloud(Game *game);

private:
    void onUpdate() override;

    Game *m_game = nullptr;
    TweenPosition *tweenPosition = nullptr;
};

#endif //SILLYTOWER_CLOUD_H
