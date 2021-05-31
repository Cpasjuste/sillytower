//
// Created by cpasjuste on 21/05/2021.
//

#include <openssl/md5.h>
#include "youlead.h"
#include "utility.h"

#ifdef __PSP2__

#include <cstring>
#include <psp2/apputil.h>
#include <psp2/system_param.h>
#include <psp2/vshbridge.h>

#define R_FAILED(res)      ((res)<0)

#endif

static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

YouLead::YouLead() {

#ifdef __PSP2__
    SceAppUtilInitParam init;
    SceAppUtilBootParam boot;
    memset(&init, 0, sizeof(SceAppUtilInitParam));
    memset(&boot, 0, sizeof(SceAppUtilBootParam));
    if (R_FAILED(sceAppUtilInit(&init, &boot))) {
        printf("ERROR: sceAppUtilInit failed\n");
        m_available = false;
        return;
    }

    // get username
    SceChar8 userName[SCE_SYSTEM_PARAM_USERNAME_MAXSIZE];
    sceAppUtilSystemParamGetString(SCE_SYSTEM_PARAM_ID_USERNAME, userName, SCE_SYSTEM_PARAM_USERNAME_MAXSIZE);
    printf("YouLead: username: %s\n", userName);

#if 0 // TODO: fix _vshSblAimgrGetConsoleId failing
    // get password
    char cid[16], id[33];
    if (R_FAILED(_vshSblAimgrGetConsoleId(cid))) {
        printf("ERROR: _vshSblAimgrGetConsoleId failed\n");
        m_available = false;
        return;
    }

    for (SceInt i = 0; i < 16; i++) {
        snprintf(&id[2 * i], 33, "%02X", cid[i]);
    }
    printf("YouLead: id: %s\n", id);
#endif

    m_user = {(char *) userName, "testpwd"};
#else
    m_user = {"cpasjuste", "testpwd"};
#endif

    curl_global_init(CURL_GLOBAL_ALL);
    m_curl = curl_easy_init();
}

User YouLead::getLocalUser() const {
    return m_user;
}

Score YouLead::getScore(const std::string &id, const std::string &username, int order) const {

    std::string readBuffer;
    std::string fields;
    Score score;

    if (!m_available || !m_curl) {
        return score;
    }

    fields = "func=GetScore";
    fields += "&id=" + id;
    fields += "&order=" + std::to_string(order);
    fields += "&name=" + username;

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_url + fields).c_str());
    //curl_easy_setopt(m_curl, CURLOPT_PROXY, "");
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5L);

    CURLcode res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return score;
    }

    std::vector<std::string> tokens = st::Utility::split(readBuffer, "<br>");
    if (tokens.size() == 2 && tokens.at(0) == "YL_OK") {
        std::string decoded = st::Utility::base64_decode(tokens.at(1).c_str(), tokens.at(1).size());
        std::vector<std::string> scoreTokens = st::Utility::split(decoded, "|n|");
        if (scoreTokens.size() == 5) {
            return Score(id, scoreTokens.at(0), std::stol(scoreTokens.at(1)), std::stoi(scoreTokens.at(2)));
        }
    } else {
        printf("error: getScore: %s\n", readBuffer.c_str());
    }

    return score;
}

std::vector<Score> YouLead::getScores(const std::string &id, int order, int limit1, int limit2) const {

    std::string readBuffer;
    std::string fields;
    std::vector<Score> scores;

    if (!m_available || !m_curl) {
        return scores;
    }

    fields = "func=GetLeaderboard";
    fields += "&id=" + id;
    fields += "&order=" + std::to_string(order);
    fields += "&limit1=" + std::to_string(limit1);
    fields += "&limit2=" + std::to_string(limit2);

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_url + fields).c_str());
    //curl_easy_setopt(m_curl, CURLOPT_PROXY, "");
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5L);

    CURLcode res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return std::vector<Score>();
    }

    std::vector<std::string> tokens = st::Utility::split(readBuffer, "<br>");
    if (tokens.size() > 1 && tokens.at(0) == "YL_OK") {
        for (size_t i = 1; i < tokens.size(); i++) {
            std::string decoded = st::Utility::base64_decode(tokens.at(i).c_str(), tokens.at(i).size());
            std::vector<std::string> scoreTokens = st::Utility::split(decoded, "|n|");
            if (scoreTokens.size() == 5) {
                scores.emplace_back(id, scoreTokens.at(0),
                                    std::stol(scoreTokens.at(1)),
                                    std::stoi(scoreTokens.at(2)));
            }
        }
    }

    return scores;
}

Score YouLead::addScore(long score) {
    return addScore(m_id, m_user.getName(), m_user.getPassword(), score);
}

Score YouLead::addScore(const std::string &id, const std::string &username, const std::string &password,
                        long _score, int order, bool overwrite) const {

    unsigned char md5[MD5_DIGEST_LENGTH];
    char hash[2 * MD5_DIGEST_LENGTH + 1];
    std::string readBuffer;
    std::string fields;
    std::string data, encoded;
    Score score;

    if (!m_available || !m_curl) {
        return score;
    }

    // base 64 data
    data = id + "|n|";
    data += std::to_string(order) + "|n|";
    data += username + "|n|";
    data += password + "|n|";
    data += std::to_string(_score) + "|n|";
    data += std::to_string((int) overwrite) + "|n|";
    encoded = st::Utility::base64_encode(data.c_str(), data.size());
    // md5
    MD5((unsigned char *) (encoded + m_key).c_str(), (encoded + m_key).size(), md5);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }

    fields = "func=AddScore";
    fields += "&data=" + encoded;
    fields += "&hash=";
    fields += hash;

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_url + fields).c_str());
    //curl_easy_setopt(m_curl, CURLOPT_PROXY, "");
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5L);

    CURLcode res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return score;
    }

    //printf("%s\n", readBuffer.c_str());
    std::vector<std::string> tokens = st::Utility::split(readBuffer, "<br>");
    if (tokens.size() == 2 && tokens.at(0) == "YL_OK") {
        std::string decoded = st::Utility::base64_decode(tokens.at(1).c_str(), tokens.at(1).size());
        std::vector<std::string> scoreTokens = st::Utility::split(decoded, "|n|");
        if (scoreTokens.size() == 5) {
            return Score(id, scoreTokens.at(0), std::stol(scoreTokens.at(1)), std::stoi(scoreTokens.at(2)));
        }
    } else {
        printf("error: addScore: %s\n", readBuffer.c_str());
    }

    return score;
}

YouLead::~YouLead() {
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
    curl_global_cleanup();
#ifdef __PSP2__
    sceAppUtilShutdown();
#endif
}
