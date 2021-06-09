//
// Created by cpasjuste on 02/06/2021.
//

#include "cross2d/c2d.h"
#include "uimusic.h"

UiMusic::UiMusic(const FloatRect &rect, const Color &color, const Color &outlineColor, Font *font)
        : RectangleShape(rect) {

    UiMusic::setOrigin(Origin::BottomRight);
    UiMusic::setFillColor(color);
    UiMusic::setOutlineColor(outlineColor);
    UiMusic::setOutlineThickness(2);
    UiMusic::setAlpha(220);
    UiMusic::setCornersRadius(5);
    UiMusic::setCornerPointCount(8);

    musicText = new Text("Playing: ?????????? @ ??????????", 18, font);
    musicText->setOrigin(Origin::TopRight);
    UiMusic::add(musicText);

    infoText = new Text("Press L/R To Change", 16, font);
    infoText->setOrigin(Origin::TopRight);
    UiMusic::add(infoText);

    UiMusic::add(new TweenPosition({UiMusic::getPosition().x, UiMusic::getPosition().y + UiMusic::getSize().y},
                                   UiMusic::getPosition(), 0.5f));
    UiMusic::setVisibility(Visibility::Hidden, false);
}

void UiMusic::show(const std::string &name, const std::string &author) {
    musicText->setString("Playing: " + name + " @ " + author);
    setSize(musicText->getSize().x + 8, getSize().y);
    musicText->setPosition(getSize().x - 4, 4);
    infoText->setPosition(getSize().x - 4, 18 + 8);
    setVisibility(Visibility::Visible, true);
    timer.restart();
}

void UiMusic::onUpdate() {
    if (isVisible() && timer.getElapsedTime().asSeconds() > 5) {
        setVisibility(Visibility::Hidden, true);
    }

    RectangleShape::onUpdate();
}
