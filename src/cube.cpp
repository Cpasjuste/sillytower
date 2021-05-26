//
// Created by cpasjuste on 20/05/2021.
//

#include "cube.h"

Cube::Cube(PhysicsWorld *world, const FloatRect &rect, const Color &color) : RectangleShape(rect) {
    Cube::setFillColor(color);
    Cube::setOutlineColor(Color::Black);
    Cube::setOutlineThickness(2);
    Cube::addPhysicsBody(world);
}

void Cube::onUpdate() {

    if (getRotation() > 15) {
        //printf("WARNING: %f\n", getRotation());
    }

    RectangleShape::onUpdate();
}
