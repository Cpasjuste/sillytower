//
// Created by cpasjuste on 30/05/2021.
//

#ifndef SILLYTOWER_MUSIC_H
#define SILLYTOWER_MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Music {

public:
    Music();

    ~Music();

    bool load(const std::string &path);

    void play();

    void pause();

    void resume();

    void unload();

private:
    Mix_Music *music = nullptr;
};

#endif //SILLYTOWER_MUSIC_H
