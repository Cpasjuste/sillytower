//
// Created by cpasjuste on 15/06/2021.
//

#ifndef SILLYTOWER_UIPAUSEMENU_H
#define SILLYTOWER_UIPAUSEMENU_H

class UiPauseMenu : public c2d::Rectangle {
public:
    explicit UiPauseMenu(Ui *ui);

    void setVisibility(Visibility visibility, bool tweenPlay = false) override;

private:
    bool onInput(Input::Player *players) override;

    void update();

    Ui *m_ui;
    int m_index = 0;
    std::vector<Text *> m_items;
    std::vector<std::string> m_texts = {
            "RESUME", "RESTART", "EXIT"
    };
};

#endif //SILLYTOWER_UIPAUSEMENU_H
