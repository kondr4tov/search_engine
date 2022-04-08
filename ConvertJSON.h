#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

#ifndef SEARCH_ENGINE_CONVERTJSON_H
#define SEARCH_ENGINE_CONVERTJSON_H

class FileIsNotOpenException: public std::exception {
    const char* what() const noexcept override {
        return "File isn't open!";
    }
};

class ConfigFileIsNotOpenException: public std::exception {
    const char* what() const noexcept override {
        return "Config file is missing!";
    }
};

class ConfigFieldIsNotFindException: public std::exception {
    const char* what() const noexcept override {
        return "Config file is empty!";
    }
};

class ConvertJSON {
public:
    ConvertJSON() = default;

    //Read configuration data from JSON
    std::string reading();
    std::vector<std::string> GetTextDocuments();
    int GetResponseLimit();
    std::vector<std::string> GetRequest();
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
    //JSON dictionary
    nlohmann::json dict;
};

#endif //SEARCH_ENGINE_CONVERTJSON_H
