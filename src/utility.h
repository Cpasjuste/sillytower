//
// Created by cpasjuste on 25/05/2021.
//

#ifndef SILLYTOWER_UTILITY_H
#define SILLYTOWER_UTILITY_H

#include <string>
#include <vector>

class Utility {

public:
    static std::vector<std::string> split(const std::string &str, const std::string &delimiter);

    static std::string base64_encode(const void *data, size_t len);

    static std::string base64_decode(const void *data, size_t len);
};

#endif //SILLYTOWER_UTILITY_H
