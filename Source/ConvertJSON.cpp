#include "../Headers/ConvertJSON.h"

std::vector<std::string> ConvertJSON::GetTextDocuments() {

    std::ifstream file("config.json");
    std::vector<std::string> vec;
    nlohmann::json j;

    if(!file.is_open()) {
        throw FileIsNotOpenException();
    }

    file >> j;
    file.close();

    for (auto it = j["files"].begin(); it != j["files"].end(); it++) {
        std::ifstream fileData;
        fileData.open(it.value());
        std::stringstream str;

        str << fileData.rdbuf();

        vec.push_back(str.str());
    }

    return vec;
};

int ConvertJSON::GetResponseLimit() {

    std::ifstream file("config.json");
    nlohmann::json j;

    if(!file.is_open()) {
        throw FileIsNotOpenException();
    }

    file >> j;

    file.close();
    return j["config"]["max_responses"];
};

std::vector<std::string> ConvertJSON::GetRequest() {

    std::ifstream file("request.json");
    nlohmann::json j;
    std::vector<std::string> vec;

    if(!file.is_open()) {
        throw FileIsNotOpenException();
    }

    file >> j;

    for(auto it = j.begin(); it != j.end(); it++) {
        for(int i = 0; i < it.value().size(); i++) {
            vec.push_back(it.value()[i]);
        }
    }
    file.close();
    return vec;
};

void ConvertJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ifstream file_in("answers.json");
    nlohmann::json j;

    if(!file_in.is_open()) {
        std::ofstream file("answers.json");
        file.close();
    }
    file_in.clear();
    file_in.close();

    std::ofstream file("answers.json");

    for(int i = 0; i < answers.size(); i++) {
        for(int k = 0; k < answers[i].size(); k++) {

            std::string str;
            if(i < 10) str = "request00" + std::to_string(i+1);
            else if (i > 9 && i < 99) str = "request0" + std::to_string(i+1);
            else str = "request" + std::to_string(i+1);

            if(answers[i][k].first != 999999999) {
                j["answers"][str] = { { "results" , true }, {"relevance" , {{"docid" , answers[i][k].first}, {"rank" , answers[i][k].second }}}};
            }
            else j["answers"][str] = { "results" , false };
        }
    }

    file << j;
    file.close();
};

