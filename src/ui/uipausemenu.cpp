//
// Created by cpasjuste on 15/06/2021.
//

#include "game.h"
#include "uipausemenu.h"

using namespace c2d;

UiPauseMenu::UiPauseMenu(Ui *ui) : Rectangle({ui->getSize().y * 0.3f, ui->getSize().y * 0.3f}) {
    m_ui = ui;
    setOrigin(Origin::Center);
    setPosition(m_ui->getSize().x / 2, m_ui->getSize().y / 2);

    float height = UiPauseMenu::getSize().y / ((float) m_texts.size() + 1);
    for (size_t i = 0; i < m_texts.size(); i++) {
        auto *text = new Text(m_texts.at(i), UI_FONT_SIZE, m_ui->getFont());
        text->setOrigin(Origin::Center);
        text->setPosition(UiPauseMenu::getSize().x / 2, height * (float) (i + 1));
        add(text);
        m_items.push_back(text);
    }

    add(new TweenScale({0, 0}, {1, 1}, 0.5f));
    update();
}

void UiPauseMenu::update() {
    for (int i = 0; i < (int) m_items.size(); i++) {
        std::string s = m_index == i ? "* " : "";
        s += m_texts.at(i);
        s += m_index == i ? " *" : "";
        m_items.at(i)->setString(s);
    }

    Rectangle::onUpdate();
}

void UiPauseMenu::setVisibility(Visibility visibility, bool tweenPlay) {
    m_index = 0;
    update();
    Rectangle::setVisibility(visibility, tweenPlay);
}

bool UiPauseMenu::onInput(Input::Player *players) {
    unsigned int keys = players[0].keys;
    if (keys & Input::Key::Up) {
        m_index--;
        if (m_index < 0) {
            m_index = (int) (m_texts.size() - 1);
        }
        update();
    } else if (keys & Input::Key::Down) {
        m_index++;
        if (m_index > (int) (m_texts.size() - 1)) {
            m_index = 0;
        }
        update();
    } else if (keys & Input::Key::Fire1) {
        if (m_index == 0) {
            m_ui->resume();
        } else if (m_index == 1) {
            m_ui->getGame()->restart();
        } else if (m_index == 2) {
            m_ui->getGame()->quit();
        }
    }

    return Rectangle::onInput(players);
}
