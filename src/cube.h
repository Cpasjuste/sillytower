//
// Created by cpasjuste on 20/05/2021.
//

#ifndef SILLYTOWER_CUBE_H
#define SILLYTOWER_CUBE_H

#include "cross2d/c2d.h"

using namespace c2d;

class Cube : public RectangleShape {

public:
    explicit Cube(const FloatRect &rect, const Color &color);

private:
    void onUpdate() override;
};

#endif //SILLYTOWER_CUBE_H
