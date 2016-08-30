#include "Decoder.h"
#include "Dictionary.h"

#include <fstream>
using std::ofstream;
#include <atomic>
using std::atomic;
#include <thread>
using std::thread;
#include <unistd.h>

ofstream output("out.txt");

const byte MIN_KEY_VALUE = 33;
const byte MAX_KEY_VALUE = 126;

atomic<size_t> keysTried(0);
const size_t totalKeys = (const size_t) pow((MAX_KEY_VALUE - MIN_KEY_VALUE), 5);

void report_thread() {
    cout << "REPORTING STARTED!" << endl;
    size_t lastRead = 0;
    while(keysTried < totalKeys) {
        cout << keysTried << "/" << totalKeys << " [" << (keysTried / totalKeys) * 100 << " %] [" << (keysTried - lastRead) / 10 << " keys/s]" << endl;
        lastRead = keysTried;
        sleep(10);
    }
}

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
            output << "KEY: " << (char*) key << ", TEXT: " << decoded << endl;
        }
        keysTried++;

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

    thread t(report_thread);
    try_decode(keyPrefix, keyPrefixSize, 16);
}