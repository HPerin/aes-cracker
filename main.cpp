#include "Decoder.h"
#include "Dictionary.h"

#include <fstream>
using std::ofstream;

ofstream output("out.txt");

const byte MIN_KEY_VALUE = 33;
const byte MAX_KEY_VALUE = 126;

void try_decode(byte keyPrefix[], size_t keyPrefixSize, size_t keyTotalSize) {
    byte key[keyTotalSize+1];
    for (size_t i = 0; i < keyPrefixSize; i++) {
        key[i] = keyPrefix[i];
    }
    for (size_t i = keyPrefixSize; i < keyTotalSize; i++) {
        key[i] = MIN_KEY_VALUE;
    }
    key[keyTotalSize] = '\0';

    Decoder decoder("encoded.txt");
    Dictionary dictionary("dict.txt");

    while (key[keyPrefixSize] <= MAX_KEY_VALUE) {
        string& decoded = decoder.decipher(key);
        if (dictionary.lookUp(decoded)) {
            cout << "KEY: " << (char*) key << ", TEXT: " << decoded << endl;
            output << "KEY: " << (char*) key << ", TEXT: " << decoded << endl;
        }

        for (size_t i = keyTotalSize-1; i >= keyPrefixSize; i--) {
            key[i]++;
            if (key[i] <= MAX_KEY_VALUE) {
                break;
            } else {
                key[i] = MIN_KEY_VALUE;
            }
        }
    }
}

int main() {
    byte keyPrefix[] = { 'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '3' };
    size_t keyPrefixSize = 11;

    try_decode(keyPrefix, keyPrefixSize, 16);
}