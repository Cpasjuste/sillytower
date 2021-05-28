//
// Created by cpasjuste on 28/05/2021.
//

#ifndef SILLYTOWER_UITOPPLAYERS_H
#define SILLYTOWER_UITOPPLAYERS_H

using namespace c2d;

class UiTopPlayers : public RectangleShape {

public:
    class ScoreItem : public RectangleShape {
    public:
        explicit ScoreItem(Ui *ui, const FloatRect &rect, Texture *cupTex,
                           const Color &cupColor, const Color &fillColor);

        Sprite *cupSprite;
        Text *rank;
        Text *name;
        Text *score;
    };

    explicit UiTopPlayers(Game *game, Ui *ui, const FloatRect &rect);

    ~UiTopPlayers() override;

    void setTopScores(const std::vector<Score> &topScores, const Score &myScore);

private:

    std::vector<ScoreItem *> scores;
    Texture *cupTex;

};

#endif //SILLYTOWER_UITOPPLAYERS_H
