//
// Created by lperin on 8/30/16.
//

#ifndef AES_CRACKER_DICTIONARY_H
#define AES_CRACKER_DICTIONARY_H

#include <string>
using std::string;
#include <vector>
using std::vector;

class Dictionary {
public:
    Dictionary(string filename);
    bool lookUp(string& text);

private:
    vector<string> dictVector;
};


#endif //AES_CRACKER_DICTIONARY_H
