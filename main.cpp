#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <exception>
#include "InvertedIndex.h"
#include "ConvertJSON.h"



//int main() {
//
//    std::string str1 = "abr.s kjnj. Azm f  dnfk  123i, jfnks jf.";
//    std::string str2 = "j. Azm f  dnfk  123i, jfnks jf.";
//    std::string str3 = "r.nmfsknzz kfnksn kjfnzzsdf,. Azm f  dnfk  123i, jfnks jf.";
//    std::string str4 = "abfsknzz  kjnj. Azm f  dnfk  123i, jfnks jf.";
//
//    InvertedIndex test;
//
//    std::vector<std::string> vec;
//
//    vec.push_back(str1);
//    vec.push_back(str2);
//    vec.push_back(str3);
//    vec.push_back(str4);
//
//    test.UpdateDocumentBase(vec);
//
//
//  return 0;
//}
