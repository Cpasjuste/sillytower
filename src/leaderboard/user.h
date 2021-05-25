//
// Created by cpasjuste on 21/05/2021.
//

#ifndef SILLYTOWER_USER_H
#define SILLYTOWER_USER_H

class User {
public:
    User() = default;

    User(const std::string &name, const std::string &pwd) {
        m_name = name;
        m_pwd = pwd;
    }

    std::string getName() const {
        return m_name;
    }

    std::string getPassword() const {
        return m_pwd;
    }

private:
    std::string m_name;
    std::string m_pwd;
};

#endif //SILLYTOWER_USER_H
