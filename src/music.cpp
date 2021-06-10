//
// Created by cpasjuste on 30/05/2021.
//

#include "game.h"
#include "music.h"

static MusicPlayer *musicInstance;

static void OnMusicFinished() {
    printf("OnMusicFinished\n");
    musicInstance->playNext();
}

MusicPlayer::MusicPlayer(Game *game) {

    musicInstance = this;
    m_game = game;

#ifdef __WITH_SDL2_MIXER__
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        if (SDL_Init(SDL_INIT_AUDIO) != 0) {
            printf("Music::Music: %s", SDL_GetError());
            return;
        }
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        printf("Music::Music: %s", Mix_GetError());
        return;
    }

    Mix_HookMusicFinished(OnMusicFinished);

    std::string path = m_game->getIo()->getRomFsPath() + "musics/";
    // GREAT
    m_playlist.emplace_back(path + "1037870_-Northstar-.mp3", "-Northstar-", "PredatorMusic");  // LICENCE OK
    m_playlist.emplace_back(path + "952516_Pink-Sand.mp3", "Pink Sand", "EX-LYD"); // LICENCE OK
    m_playlist.emplace_back(path + "1027248_Arcade-Bounce.mp3", "Arcade Bounce", "DuttonsaysHi"); // LICENCE OK
    m_playlist.emplace_back(path + "1037651_Everhood---Feisty-Flowers.mp3", "Feisty Flowers", "cazok"); // LICENCE OK
    m_playlist.emplace_back(path + "1031157_Duel-of-the-Poets.mp3", "Duel of the Poets", "dullsworth"); // LICENCE OK
    m_playlist.emplace_back(path + "969284_-Pirate-Fox-.mp3", "-Pirate Fox-", "PredatorMusic"); // LICENCE OK
    m_playlist.emplace_back(path + "1038025_Everhood---The-Final-Battl.mp3", "The Final Battle", "cazok"); // LICENCE OK
    m_playlist.emplace_back(path + "1002761_-Slime-Factory-.mp3", "-Slime Factory-", "PredatorMusic"); // LICENCE OK
    //m_playlist.emplace_back(path + "", "", "");

    m_available = true;
#endif
}

bool MusicPlayer::playNext() {

    m_playlistPos++;
    if (m_playlistPos >= (int) m_playlist.size()) {
        m_playlistPos = 0;
    }

    return play(m_playlistPos);
}

bool MusicPlayer::playPrev() {

    m_playlistPos--;
    if (m_playlistPos < 0) {
        m_playlistPos = (int) m_playlist.size() - 1;
    }

    return play(m_playlistPos);
}

bool MusicPlayer::play(size_t pos) {

    if (!m_available) {
        return false;
    }

#ifdef __WITH_SDL2_MIXER__
    if (m_music) {
        Mix_FreeMusic(m_music);
        m_music = nullptr;
    }

    if (pos < 0 || pos >= m_playlist.size()) {
        printf("ERROR: Music::play: %li\n", pos);
        return false;
    }

    printf("Music::play: %li - %s\n", pos, m_playlist.at(pos).m_path.c_str());
    m_music = Mix_LoadMUS(m_playlist.at(pos).m_path.c_str());
    if (!m_music) {
        printf("ERROR: Music::play: %s\n", Mix_GetError());
        return false;
    }

    int res = Mix_PlayMusic(m_music, 0);
    if (res < 0) {
        printf("ERROR: Music::play: res = %i, error: %s\n", res, Mix_GetError());
        return false;
    }

    m_game->getUi()->showMusicPlayer(m_playlist.at(pos).m_name, m_playlist.at(pos).m_author);

    return true;
#endif
}

void MusicPlayer::pause() {
    if (!m_available) {
        return;
    }
#ifdef __WITH_SDL2_MIXER__
    if (m_music) {
        Mix_PauseMusic();
    }
#endif
}

void MusicPlayer::resume() {
    if (!m_available) {
        return;
    }
#ifdef __WITH_SDL2_MIXER__
    if (m_music) {
        Mix_ResumeMusic();
    }
#endif
}

MusicPlayer::~MusicPlayer() {
#ifdef __WITH_SDL2_MIXER__
    Mix_HookMusicFinished(nullptr);
    if (m_music) {
        Mix_FreeMusic(m_music);
    }
    Mix_CloseAudio();
    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
#endif
}
