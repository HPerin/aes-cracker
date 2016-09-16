//
// Created by lperin on 8/30/16.
//

#include "Decoder.h"

#include <fstream>
using std::ifstream;
using std::getline;

Decoder::Decoder(vector<string> encoded, bool (*isValid)(string&)) {
    this->isValid = isValid;

    for (string line : encoded) {
        string hexDecoded;
        StringSource(line, true,
            new HexDecoder(
                    new StringSink(hexDecoded)
            ));
        this->encoded.push_back(hexDecoded);
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







