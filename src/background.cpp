//
// Created by cpasjuste on 20/05/2021.
//

#include "background.h"

Background::Background(const FloatRect &rect) {

    Sprite::setTextureRect({0, 0, (int) rect.width, (int) rect.height});
    Sprite::setPosition(rect.left, rect.top);

    auto *vertices = getVertexArray()->getVertices();
    vertices->at(0).color = Color::Black;
    vertices->at(1).color = Color(90, 222, 255);
    vertices->at(2).color = Color::Black;
    vertices->at(3).color = Color(90, 222, 255);
    getVertexArray()->update();
}
