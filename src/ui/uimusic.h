//
// Created by cpasjuste on 02/06/2021.
//

#ifndef SILLYTOWER_UIMUSIC_H
#define SILLYTOWER_UIMUSIC_H

using namespace c2d;

class UiMusic : public RectangleShape {

public:
    explicit UiMusic(const FloatRect &rect, const Color &color, const Color &outlineColor, Font *font);

    void show(const std::string &name, const std::string &author);

private:
    void onUpdate() override;

    Text *musicText;
    Text *infoText;
    C2DClock timer;
};

#endif //SILLYTOWER_UIMUSIC_H
