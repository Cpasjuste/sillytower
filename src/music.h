//
// Created by cpasjuste on 30/05/2021.
//

#ifndef SILLYTOWER_MUSIC_H
#define SILLYTOWER_MUSIC_H

#ifdef __WITH_SDL2_MIXER__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <string>

class MusicPlayer {

public:
    class PlaylistItem {
    public:
        PlaylistItem(const std::string &path, const std::string &name, const std::string &author) {
            m_path = path;
            m_name = name;
            m_author = author;
        }

        std::string m_path;
        std::string m_name;
        std::string m_author;
    };

    explicit MusicPlayer(Game *game);

    ~MusicPlayer();

    bool play(size_t pos);

    bool playNext();

    bool playPrev();

    void pause();

    void resume();

private:

    Game *m_game;
    std::vector<PlaylistItem> m_playlist;
    int m_playlistPos = 0;
    bool m_available = false;
#ifdef __WITH_SDL2_MIXER__
    Mix_Music *m_music = nullptr;
#endif
};

#endif //SILLYTOWER_MUSIC_H
