//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_YOULEAD_H
#define SILLYTOWER_YOULEAD_H

#include <string>
#include <vector>
#include <curl/curl.h>
#include "score.h"

class YouLead {

public:
    YouLead();

    ~YouLead();

    Score getScore() const;

    std::vector<Score> getScores() const;

private:
    CURL *curl = nullptr;
    std::string key = __YOULEAD_KEY__;
    std::string url = "http://mydedibox.fr/sillytower/leaderboard/Unity.php?";
};

#endif //SILLYTOWER_YOULEAD_H
