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
    //UiTopPlayers::setCornersRadius(5);
    //UiTopPlayers::setCornerPointCount(8);

    Text *title = new Text("TOP SCORES", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    title->setOrigin(Origin::BottomRight);
    title->setPosition(rect.width - 8, 1);
    UiTopPlayers::add(title);

    // scores items
    m_cupTex = new C2DTexture(game->getIo()->getRomFsPath() + "cup.png");
    float itemHeight = rect.height / 3;
    // my score item
    m_myScore = new ScoreItem(ui, {0, -(itemHeight * 1.75f), rect.width, itemHeight}, m_cupTex,
                              {254, 225, 1}, game->getColor(Game::SillyColor::yellow));
    m_myScore->setOutlineColor(Color::Black);
    m_myScore->setOutlineThickness(2);
    m_myScore->rank->setVisibility(Visibility::Visible);
    m_myScore->cupSprite->setVisibility(Visibility::Hidden);
    Text *myScoreTitle = new Text("MY BEST SCORE", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    myScoreTitle->setOrigin(Origin::BottomRight);
    myScoreTitle->setPosition(m_myScore->getSize().x - 8, 1);
    m_myScore->add(myScoreTitle);
    UiTopPlayers::add(m_myScore);
    // top scores item
    // 1
    auto *item = new ScoreItem(ui, {0, 0, rect.width, itemHeight}, m_cupTex,
                               {254, 225, 1}, game->getColor(Game::SillyColor::blue));
    UiTopPlayers::add(item);
    m_topScores.push_back(item);
    // 2
    item = new ScoreItem(ui, {0, itemHeight, rect.width, itemHeight}, m_cupTex,
                         {167, 167, 173}, game->getColor(Game::SillyColor::magenta));
    UiTopPlayers::add(item);
    m_topScores.push_back(item);
    // 3
    item = new ScoreItem(ui, {0, itemHeight * 2, rect.width, itemHeight}, m_cupTex,
                         {167, 112, 68}, game->getColor(Game::SillyColor::green));
    UiTopPlayers::add(item);
    m_topScores.push_back(item);
}

void UiTopPlayers::setTopScores(const std::vector<Score> &topScores, const Score &myScore) {
    // top scores
    for (size_t i = 0; i < topScores.size(); i++) {
        std::string upper = c2d::Utility::toUpper(topScores.at(i).getUsername());
        m_topScores.at(i)->name->setString(upper);
        m_topScores.at(i)->score->setString(std::to_string(topScores.at(i).getScore()));
    }
    // my score
    std::string upper = c2d::Utility::toUpper(myScore.getUsername());
    m_myScore->name->setString(upper);
    m_myScore->rank->setString(std::to_string(myScore.getRank()));
    m_myScore->score->setString(std::to_string(myScore.getScore()));
}

UiTopPlayers::~UiTopPlayers() {
    delete m_cupTex;
}

UiTopPlayers::ScoreItem::ScoreItem(Ui *ui, const FloatRect &rect, Texture *cupTex,
                                   const Color &cupColor, const Color &fillColor) : RectangleShape(rect) {

    UiTopPlayers::ScoreItem::setFillColor(fillColor);
    //UiTopPlayers::ScoreItem::setCornersRadius(5);
    //UiTopPlayers::ScoreItem::setCornerPointCount(8);

    cupSprite = new Sprite(cupTex);
    cupSprite->setPosition(8, 8);
    cupSprite->setFillColor(cupColor);
    UiTopPlayers::ScoreItem::add(cupSprite);

    rank = new Text("??", 18, ui->getFont());
    rank->setOrigin(Origin::Center);
    rank->setPosition(8 + (cupSprite->getSize().x / 2), rect.height / 2);
    rank->setVisibility(Visibility::Hidden);
    UiTopPlayers::ScoreItem::add(rank);

    name = new Text("NO CONNEXION !", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    name->setOrigin(Origin::Left);
    name->setPosition(64, rect.height / 2);
    UiTopPlayers::ScoreItem::add(name);

    score = new Text("??", C2D_DEFAULT_CHAR_SIZE, ui->getFont());
    score->setOrigin(Origin::Right);
    score->setPosition(rect.width - 8, rect.height / 2);
    UiTopPlayers::ScoreItem::add(score);
}
