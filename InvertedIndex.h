#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <exception>
#include <filesystem>
#include <map>
#include <thread>

#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

struct Entry {
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);
    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный    словарь
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
