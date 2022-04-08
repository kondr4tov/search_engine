#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase (std::vector<std::string> input_docs) {

    std::string path = "/Users/andrejkondratov/CLionProjects/search_engine/FilesToIndex";
    input_docs.clear();

    for (auto & p : std::__fs::filesystem::directory_iterator(path)) {
        input_docs.push_back(p.path());
    }

    //Copy vector to docs
    input_docs = docs;
};

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {

    Entry entry;
    std::vector<Entry> vec;

    for(int i = 0; i < docs.size(); i++) {
        std::ifstream file(docs[i]);

        std::string currentWord;
        int count = 0;

        while (!file.eof()) {
            file >> currentWord;
            count += (currentWord == word);
        }

        entry.count = count;
        entry.doc_id = i;
        vec.push_back(entry);
        freq_dictionary.insert(std::pair<std::string, std::vector<Entry>>(word, vec));

        file.close();
    }
};

