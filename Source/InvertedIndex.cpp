#include "../Headers/InvertedIndex.h"
#include <gtest/gtest.h>
#include <mutex>

using namespace std;
void TestInvertedIndexFunctionality(
        const vector<string>& docs,
        const vector<string>& requests,
        const std::vector<vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
const vector<string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
};
const vector<string> requests = {"london", "the"};
const vector<vector<Entry>> expected = {
        {
                {0, 1}
        }, {
                {0, 1}, {1, 3}
        }
};
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "Americano Cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuccino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

std::mutex m;

void IndexInsideUpdate(std::string str, int numberOfDoc, std::map<std::string, std::vector<Entry>>& freq_dictionary) {

    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(c); });

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
    Entry entry;

    //fill the map
    for(auto i : result) {
        std::vector<Entry> toMapVec;
        if(myMap.count(i) == 0) {
            entry.doc_id = numberOfDoc;
            entry.count = 1;
            toMapVec.push_back(entry);
            myMap.insert(std::make_pair(i,toMapVec));
        }
        else {
            myMap.find(i)->second[0].count++;
        }
    }

    //block threads and fill dictionary
    m.lock();
    for(auto it = myMap.begin(); it != myMap.end(); it++) {
        if(freq_dictionary.count(it->first) == 0) {
            freq_dictionary.insert(std::make_pair(it->first, it->second));
        }
        else {
            Entry entry1;
            entry1.count = myMap.find(it->first)->second[0].count;
            entry1.doc_id = myMap.find(it->first)->second[0].doc_id;
            freq_dictionary.find(it->first)->second.push_back(entry1);
        }
    }
    m.unlock();

}

void InvertedIndex::UpdateDocumentBase (std::vector<std::string> input_docs) {

    //vector of threads
    std::vector<std::thread> th_vec;
    std::mutex mutex;


    //initialization of threads
    for(int i = 0; i < input_docs.size(); i++) {
        mutex.lock();
        th_vec.push_back(std::thread(IndexInsideUpdate, input_docs[i], i, std::ref(freq_dictionary)));
        mutex.unlock();
    }
    //join to all threads
    for(int i = 0; i < input_docs.size(); i++) {
        th_vec[i].join();
    }
};

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {

    Entry entry;
    std::vector<Entry> vec = {};

    for(auto it = freq_dictionary.begin(); it != freq_dictionary.end(); it++) {
        std::sort(it->second.begin(), it->second.end(), [](const Entry& dot1, const Entry& dot2) -> bool
                  {
                      return dot1.doc_id < dot2.doc_id;
                  }
        );
    }

    if(freq_dictionary.count(word) != 0) {
        for (int i = 0; i < freq_dictionary.find(word)->second.size(); i++) {
            entry = freq_dictionary.find(word)->second[i];
            vec.push_back(entry);
        }
    }

    return vec;
};

