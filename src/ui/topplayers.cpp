//
// Created by cpasjuste on 28/05/2021.
//

#include "cross2d/c2d.h"
#include "game.h"
#include "topplayers.h"

UiTopPlayers::UiTopPlayers(Game *game, Ui *ui, const FloatRect &rect) : RectangleShape(rect) {
    UiTopPlayers::setFillColor(Color::Transparent);
    UiTopPlayers::setOutlineColor(Color::Black);
    UiTopPlayers::setOutlineThickness(2);

    // top scores items
    cupTex = new C2DTexture(game->getIo()->getRomFsPath() + "cup.png");
    float itemHeight = rect.height / 4;
    // 0
    auto *item = new ScoreItem(ui, {0, 0, rect.width, itemHeight}, cupTex,
                               {254, 225, 1}, game->getColor(Game::SillyColor::blue));
    UiTopPlayers::add(item);
    scores.push_back(item);
    // 1
    item = new ScoreItem(ui, {0, itemHeight, rect.width, itemHeight}, cupTex,
                         {167, 167, 173}, game->getColor(Game::SillyColor::magenta));
    UiTopPlayers::add(item);
    scores.push_back(item);
    // 2
    item = new ScoreItem(ui, {0, itemHeight * 2, rect.width, itemHeight}, cupTex,
                         {167, 112, 68}, game->getColor(Game::SillyColor::green));
    UiTopPlayers::add(item);
    scores.push_back(item);
    // 3
    item = new ScoreItem(ui, {0, itemHeight * 3, rect.width, itemHeight}, cupTex,
                         Color::White, game->getColor(Game::SillyColor::yellow));
    UiTopPlayers::add(item);
    scores.push_back(item);
}

void UiTopPlayers::setTopScores(const std::vector<Score> &topScores, const Score &myScore) {
    // top scores
    for (size_t i = 0; i < topScores.size(); i++) {
        std::string upper = c2d::Utility::toUpper(topScores.at(i).getUsername());
        scores.at(i)->name->setString(upper);
        scores.at(i)->rank->setString(std::to_string(topScores.at(i).getRank()));
        scores.at(i)->score->setString(std::to_string(topScores.at(i).getScore()));
    }
    // my score
    std::string upper = c2d::Utility::toUpper(myScore.getUsername());
    scores.at(3)->name->setString(upper);
    scores.at(3)->rank->setString(std::to_string(myScore.getRank()));
    scores.at(3)->score->setString(std::to_string(myScore.getScore()));
}

UiTopPlayers::~UiTopPlayers() {
    delete cupTex;
}

UiTopPlayers::ScoreItem::ScoreItem(Ui *ui, const FloatRect &rect, Texture *cupTex,
                                   const Color &cupColor, const Color &fillColor) : RectangleShape(rect) {

    UiTopPlayers::ScoreItem::setFillColor(fillColor);

    cupSprite = new Sprite(cupTex);
    cupSprite->setPosition(8, 0);
    cupSprite->setFillColor(cupColor);
    UiTopPlayers::ScoreItem::add(cupSprite);

    rank = new Text("0", 18, ui->getFont());
    rank->setOrigin(Origin::Top);
    rank->setPosition(8 + (cupSprite->getSize().x / 2), cupSprite->getSize().y - 4);
    UiTopPlayers::ScoreItem::add(rank);

    name = new Text("NO SCORE YET", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    name->setOrigin(Origin::Left);
    name->setPosition(64, rect.height / 2);
    UiTopPlayers::ScoreItem::add(name);

    score = new Text("0", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    score->setOrigin(Origin::Right);
    score->setPosition(rect.width - 8, rect.height / 2);
    UiTopPlayers::ScoreItem::add(score);
}
