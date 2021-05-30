//
// Created by cpasjuste on 30/05/2021.
//

#include "music.h"

Music::Music() {

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_Init(SDL_INIT_AUDIO);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
        printf("Music::Music: %s", Mix_GetError());
        return;
    }
}

bool Music::load(const std::string &path) {
    music = Mix_LoadMUS(path.c_str());
    return music != nullptr;
}

void Music::play() {
    if (music) {
        Mix_PlayMusic(music, -1);
    }
}

void Music::pause() {
    if (music) {
        Mix_PauseMusic();
    }
}

void Music::resume() {
    if (music) {
        Mix_ResumeMusic();
    }
}

void Music::unload() {
    if (music) {
        Mix_PauseMusic();
        Mix_FreeMusic(music);
    }
}

Music::~Music() {
    if (music) {
        Mix_FreeMusic(music);
    }

    Mix_CloseAudio();

    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}
