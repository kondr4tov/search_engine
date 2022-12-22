#include "../Headers/SearchServer.h"
#include "../Headers/ConvertJSON.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {

    std::vector<std::vector<RelativeIndex>> vecToOutput;

    for(int j = 0; j < queries_input.size(); j++) {

        std::string bufferStr;
        std::transform(queries_input[j].begin(), queries_input[j].end(), std::back_inserter(bufferStr), [](const char& c) {return std::tolower(c);});

        std::stringstream s(bufferStr);
        std::string word;
        std::vector<std::string> result;

        //divide to word
        while(s >> word) {
            result.push_back(word);
        }

        std::map<std::string, int> uniqList;

        for(auto i : result) {
            size_t counter = 0;
            if(uniqList.count(i) == 0) {
                if(_index.freq_dictionary.count(i) != 0) {
                    for(auto entry : _index.freq_dictionary.find(i)->second) {
                        counter += entry.count;
                    }
                    uniqList.insert(std::make_pair(i, counter));
                }
                else {
                    uniqList.insert(std::make_pair(i, counter));
                }
            }
        }

        std::multimap<int, std::string> reverseUniqList;

        for(auto it = uniqList.begin(); it != uniqList.end(); it++) {
            reverseUniqList.insert(std::make_pair(it->second, it->first));
        }

        std::vector<int> docs;
        std::vector<RelativeIndex> relativeIndexVec;
        std::multimap<int, int> toRelevant;
        RelativeIndex relativeIndex;

        auto it1 = reverseUniqList.rbegin();
        if(_index.freq_dictionary.count(it1->second) == 0) {
            RelativeIndex emptyRelInd;
            relativeIndexVec.push_back(emptyRelInd);
            vecToOutput.push_back(relativeIndexVec);
            continue;
        }
        else {
            for (auto words: _index.freq_dictionary.find(it1->second)->second) {
                docs.push_back(words.doc_id);
            }
        }

        ConvertJSON convertJson;

        for(int i = 0; i < docs.size(); i++) {
            int counter = 0;
            for(auto it = reverseUniqList.begin(); it != reverseUniqList.end(); it++) {
                for(int m = 0; m < _index.freq_dictionary.find(it->second)->second.size(); m++) {
                    if(_index.freq_dictionary.find(it->second)->second[m].doc_id == docs[i]) {
                        counter += _index.freq_dictionary.find(it->second)->second[m].count;
                    }
                }
            }
            std::pair<int, int> myPair(std::make_pair(counter, docs[i]));
            toRelevant.insert(myPair);
        }

        auto it2 = toRelevant.rbegin();
        float absRel = it2->first;

        for(auto it = toRelevant.begin(); it != toRelevant.end(); it++) {
            float otnRel = (it->first / absRel) + 0.04;
            std::string helperStr = std::to_string(otnRel);
            std::stringstream out;
            out << std::fixed << std::setprecision(1) << std::stof(helperStr);
            float otnRel2 = std::stof(out.str());
            relativeIndex.doc_id = it->second;
            relativeIndex.rank = otnRel2;
            relativeIndexVec.push_back(relativeIndex);
        }

        std::sort(relativeIndexVec.begin(), relativeIndexVec.end(), more_than_rank());

        std::sort(relativeIndexVec.begin(), relativeIndexVec.end(), [](const RelativeIndex& dot1, const RelativeIndex& dot2) -> bool
                  {
                      if(dot1.rank == dot2.rank) return dot1.doc_id < dot2.doc_id;
                  }
        );

        if(relativeIndexVec.size() > 4) {
            std::vector<RelativeIndex> bufferVector(relativeIndexVec.begin(),
                                                    relativeIndexVec.begin() + convertJson.GetResponseLimit());
            vecToOutput.push_back(bufferVector);
        }
        else {
            vecToOutput.push_back(relativeIndexVec);
        }
    }

    return vecToOutput;
}