//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_SCORE_H
#define SILLYTOWER_SCORE_H

class Score {

public:
    Score() = default;;

    Score(const std::string &id, const std::string &username, long score, int rank, void *data = nullptr) {
        m_id = id;
        m_username = username;
        m_score = score;
        m_rank = rank;
        m_data = data;
    }

    std::string getId() const {
        return m_id;
    }

    std::string getUsername() const {
        return m_username;
    }

    long getScore() const {
        return m_score;
    }

    int getRank() const {
        return m_rank;
    }

    void *getData() const {
        return m_data;
    }

private:
    std::string m_id;
    std::string m_username;
    long m_score = 0;
    int m_rank = 0;
    void *m_data = nullptr;
};

#endif //SILLYTOWER_SCORE_H
