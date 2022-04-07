#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <exception>
#include "InvertedIndex.h"
#include "ConvertJSON.h"

int main() {

    InvertedIndex test;
    std::vector<std::string> vec;

    test.UpdateDocumentBase(vec);

    return 0;
}
