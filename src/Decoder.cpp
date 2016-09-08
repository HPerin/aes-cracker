//
// Created by lperin on 8/30/16.
//

#include "Decoder.h"

#include <fstream>
using std::ifstream;
using std::getline;

Decoder::Decoder(string filename, bool (*isValid)(string&)) {
    this->isValid = isValid;

    ifstream file(filename);

    string line;
    while(getline(file, line)) {
        string hexDecoded;
        StringSource(line, true,
            new HexDecoder(
                    new StringSink(hexDecoded)
            ));
        encoded.push_back(hexDecoded);
    }
}

string& Decoder::decipher(byte key[]) {
    for (unsigned i = 0; i < AES_KEY_LENGTH; i++) {
        this->key[i] = key[i];
    }
    decoder.SetKey(key, AES_KEY_LENGTH);

    decoded = "";
    for (string &cur : encoded) {
        string curDecoded;
        StringSource(cur, true,
                     new StreamTransformationFilter(decoder,
                                                    new StringSink(curDecoded),
                                                    StreamTransformationFilter::NO_PADDING));
        if (isValid(curDecoded)) {
            decoded += curDecoded;
        } else {
            decoded = "";
            return decoded;
        }
    }
    return decoded;
}







