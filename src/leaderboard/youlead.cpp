//
// Created by cpasjuste on 21/05/2021.
//

#include "youlead.h"

static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

YouLead::YouLead() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, "http://mydedibox.fr/sillytower/leaderboard/Unity.php?func=GetScore");
    }
}

Score YouLead::getScore() const {

    if (!curl) {
        return Score();
    }

    std::string readBuffer;

    std::string fields = "func=GetScore";
    fields += "&id=top";
    fields += "&order=0";
    fields += "&name=cpasjuste";

    curl_easy_setopt(curl, CURLOPT_URL, (url + fields).c_str());

    // TODO: remove
    curl_easy_setopt(curl, CURLOPT_PROXY, "proxy-eple.in.ac-nantes.fr:3128");
    // TODO: remove

    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return Score();
    }

    printf("%s\n", readBuffer.c_str());

    return Score();
}

std::vector<Score> YouLead::getScores() const {

    if (!curl) {
        return std::vector<Score>();
    }

    std::string readBuffer;

    std::string fields = "func=GetLeaderboard";
    fields += "&id=top";
    fields += "&order=0";
    fields += "&limit1=0";
    fields += "&limit2=0";

    curl_easy_setopt(curl, CURLOPT_URL, (url + fields).c_str());

    // TODO: remove
    //curl_easy_setopt(curl, CURLOPT_PROXY, "");
    // TODO: remove

    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return std::vector<Score>();
    }

    printf("%s\n", readBuffer.c_str());

    return std::vector<Score>();
}

YouLead::~YouLead() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}
