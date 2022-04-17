#include "../Headers/SearchServer.h"
#include <gtest/gtest.h>

using namespace std;
TEST(TestCaseSearchServer, TestSimple) {
const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "Americano Cappuccino"
};
const vector<string> request = {"milk water", "sugar"};
const std::vector<vector<RelativeIndex>> expected = {
        {
                {2, 1},
                {0, 0.7},
                {1, 0.3}
        },
        {
                {}
        }
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
std::vector<vector<RelativeIndex>> result = srv.search(request);
ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
const vector<string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
};
const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.66},
                    {1, 0.66},
                    {2, 0.66}
            }
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
std::vector<vector<RelativeIndex>> result = srv.search(request);
ASSERT_EQ(result, expected);
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {

    std::vector<std::vector<RelativeIndex>> vecToOutput;

    for(int j = 0; j < queries_input.size(); j++) {

        std::transform(queries_input[j].begin(), queries_input[j].end(), queries_input[j].begin(),
                       [](unsigned char c) -> unsigned char { return std::tolower(c); });

        std::stringstream s(queries_input[j]);
        std::string word;
        std::vector<std::string> result;

        //divide to word
        while(s >> word) {
            result.push_back(word);
        }

        std::map<std::string, int> uniqList;

        for(auto i : result) {
            size_t count = 0;
            if(uniqList.count(i) == 0) {
                if(_index.freq_dictionary.count(i) != 0) {
                    for(auto entry : _index.freq_dictionary.find(i)->second) {
                        count += entry.count;
                    }
                    uniqList.insert(std::make_pair(i, count));
                }
                else {
                    uniqList.insert(std::make_pair(i, count));
                }
            }
        }

        std::map<int, std::string> reverseUniqList;

        for(auto it = uniqList.begin(); it != uniqList.end(); it++) {
            reverseUniqList.insert(std::make_pair(it->second, it->first));
        }

        std::vector<int> docs;

        auto ut = reverseUniqList.begin();
        for(auto words : _index.freq_dictionary.find(ut->second)->second) {
            docs.push_back(words.doc_id);
        }

        std::vector<RelativeIndex> relativeIndexVec;

        for(int i = 0; i < docs.size(); i++) {
            int counter = 0;
            int absRel = 0;
            int otnRel;
            RelativeIndex relativeIndex;
            for(auto it = reverseUniqList.begin(); it != reverseUniqList.end(); it++) {
                for(int m = 0; m < _index.freq_dictionary.find(it->second)->second.size(); m++) {
                    if(_index.freq_dictionary.find(it->second)->second[m].doc_id == docs[i]) {
                        counter += _index.freq_dictionary.find(it->second)->second[m].count;
                    }
                }
                if(counter >= absRel) absRel = counter;
                //here have to be else for false
                otnRel = counter / absRel;
                relativeIndex.doc_id = docs[i];
                relativeIndex.rank = otnRel;
                relativeIndexVec.push_back(relativeIndex);
            }
        }

        std::sort(relativeIndexVec.begin(), relativeIndexVec.end(), more_than_rank());

        vecToOutput.push_back(relativeIndexVec);
    }

    return vecToOutput;
}