#include "InvertedIndex.h"

void IndexInsideUpdate(std::string str, int numberOfDoc, std::map<std::string, std::vector<Entry>>& freq_dictionary) {

    std::transform(str.begin(), str.end(), str.begin(), tolower);

    //remove punctuation
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '.' || str[i] == ',' || str[i] == '!' || str[i] == '?' || str[i] == ';' || str[i] == ':'
               || str[i] == '"' || str[i] == '/' || str[i] == '|' || str[i] == '*' || str[i] == '*' || str[i] == '#') {
            str[i] = ' ';
        }
    }

    std::stringstream s(str);
    std::string word;
    std::vector<std::string> result;

    //divide to word
    while(s >> word) {
        result.push_back(word);
    }

    std::map<std::string, std::vector<Entry>> myMap;
    std::vector<Entry> toMapVec;
    Entry entry;
    std::mutex m;

    //fill the map
    for(auto i : result) {
        if(myMap.count(i) == 0) {
            entry.doc_id = numberOfDoc;
            entry.count = 1;
            toMapVec.push_back(entry);
            myMap.insert(std::make_pair(i,toMapVec));
        }
        else {
            myMap.find(i)->second[0].count += 1;
        }
    }

    //block threads and fill dictionary
    m.lock();
    for(auto it = myMap.begin(); it != myMap.end(); it++) {
        if(freq_dictionary.count(it->first) == 0) {
            freq_dictionary.insert(std::make_pair(it->first, toMapVec));
        }
        else {
            freq_dictionary.find(it->first)->second.push_back(myMap.find(it->first)->second[0]);
        }
    }
    m.unlock();

}

void InvertedIndex::UpdateDocumentBase (std::vector<std::string> input_docs) {

    //vector of threads
    std::vector<std::thread> th_vec;

    //initialization of threads
    for(int i = 0; i < input_docs.size(); i++) {
        std::thread th(IndexInsideUpdate, input_docs[i], i, freq_dictionary);
        th_vec.push_back(th);
    }

    //join to all threads
    for(int i = 0; i < input_docs.size(); i++) {
        th_vec[i].join();
    }
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
    return vec;
};

