//
// Created by cpasjuste on 25/05/2021.
//

#ifndef SILLYTOWER_UTILITY_H
#define SILLYTOWER_UTILITY_H

#include "cross2d/c2d.h"
#include <string>
#include <vector>

namespace sillytower {

    class Utility {

    public:
        static c2d::IntRect getTextureRect(c2d::Texture *texture, int rows, int x, int y);

        static std::vector<std::string> split(const std::string &str, const std::string &delimiter);

        static std::string base64_encode(const void *data, size_t len);

        static std::string base64_decode(const void *data, size_t len);
    };
}

#endif //SILLYTOWER_UTILITY_H
