#include <iostream>
#include "InvertedIndex.h"

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

struct more_than_rank
{
    inline bool operator() (const RelativeIndex& struct1, const RelativeIndex& struct2)
    {
        return (struct1.rank > struct2.rank);
    }
};

class SearchServer {
public:

    SearchServer(InvertedIndex& idx) : _index(idx){ };

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:

    InvertedIndex _index;
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
