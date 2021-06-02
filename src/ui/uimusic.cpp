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
    musicText->setPosition(UiMusic::getSize().x - 4, 4);
    UiMusic::add(musicText);

    infoText = new Text("Press L/R To Change", 16, font);
    infoText->setOrigin(Origin::TopRight);
    infoText->setPosition(UiMusic::getSize().x - 4, 18 + 8);
    UiMusic::add(infoText);

    UiMusic::setSize((float) (musicText->getString().size() * 11), rect.height);
    musicText->setPosition(UiMusic::getSize().x - 4, 4);
    infoText->setPosition(UiMusic::getSize().x - 4, 18 + 8);

    UiMusic::add(new TweenPosition({UiMusic::getPosition().x, UiMusic::getPosition().y + UiMusic::getSize().y},
                                   UiMusic::getPosition(), 0.5f));
    UiMusic::setVisibility(Visibility::Hidden, false);

    timer = new C2DClock();
}

void UiMusic::show(const std::string &name, const std::string &author) {
    musicText->setString("Playing: " + name + " @ " + author);
    setSize((float) (musicText->getString().size() * 11), getSize().y);
    musicText->setPosition(getSize().x - 4, 4);
    infoText->setPosition(getSize().x - 4, 18 + 8);
    setVisibility(Visibility::Visible, true);
    timer->restart();
}

void UiMusic::onUpdate() {
    if (isVisible() && timer->getElapsedTime().asSeconds() > 5) {
        setVisibility(Visibility::Hidden, true);
    }

    RectangleShape::onUpdate();
}

UiMusic::~UiMusic() {
    delete (timer);
}
