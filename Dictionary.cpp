//
// Created by lperin on 8/30/16.
//

#include "Dictionary.h"

#include <fstream>
using std::ifstream;
#include <algorithm>
using std::binary_search;

Dictionary::Dictionary(string filename) {
    ifstream file(filename);

    string line;
    while(getline(file, line)) {
        dictVector.push_back(line);
    }
}

bool Dictionary::lookUp(string& text) {
    for (string& word : dictVector) {
        if (text.find(word) != std::string::npos) {
            return true;
        }
    }

    return false;
}

