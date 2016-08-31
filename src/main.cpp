#include "Decoder.h"
#include "Dictionary.h"

#include <fstream>
using std::ofstream;
#include <atomic>
using std::atomic;
#include <thread>
using std::thread;
using std::bind;
#include <string>
using std::to_string;
#include <unistd.h>

ofstream output("out.txt");

const byte MIN_KEY_VALUE = 33;
const byte MAX_KEY_VALUE = 126;
const size_t MAX_THREADS = 4;

atomic<size_t> keysTried(0);
atomic<size_t> validKeys(0);
atomic<size_t> runningThreads(0);
const size_t totalKeys = (const size_t) pow((MAX_KEY_VALUE - MIN_KEY_VALUE), 5);

void report_thread() {
    cout << "REPORTING STARTED!" << endl;
    size_t lastRead = 0;
    while(keysTried < totalKeys) {
        double keysPerSec = (double(keysTried) - double(lastRead)) / 10;
        double secondsLeft = (double(totalKeys) - double(keysTried)) / (keysPerSec);
        string timeLeft = "";
        if (secondsLeft > 86400) timeLeft = to_string(secondsLeft / 86400) + " days";
        else if (secondsLeft > 3600) timeLeft = to_string(secondsLeft / 3600) + " hours";
        else if (secondsLeft > 60) timeLeft = to_string(secondsLeft / 60) + " minutes";
        else timeLeft = to_string(secondsLeft) + " seconds";
        cout << "\r" << validKeys << " valid keys | " << keysTried << "/" << totalKeys << " [" << (double(keysTried) / double(totalKeys)) * 100 << " %] [" << keysPerSec << " keys/s] [" << timeLeft << " left]" << endl;
        lastRead = keysTried;
        sleep(10);
    }
}

inline bool isValid(string& text) {
    for (char& c : text) {
        if ((c < 32 || c > 126) && c != 0)
            return false;
    }

    return true;
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

    while (key[keyPrefixSize] <= MAX_KEY_VALUE) {
        string& decoded = decoder.decipher(key);

        if (isValid(decoded)) {
            validKeys++;
            output << "KEY: " << (char *) key << ", TEXT: " << decoded << endl;
        }
        keysTried++;

        for (size_t i = keyTotalSize-1; i >= keyPrefixSize; i--) {
            key[i]++;
            if (key[i] <= MAX_KEY_VALUE) {
                break;
            } else if (i != keyPrefixSize){
                key[i] = MIN_KEY_VALUE;
            }
        }
    }

    runningThreads--;
}

int main() {
    //byte keyPrefix[] = { 'e', 's', 's', 'a', 's', 'e', 'n', 'h', 'a', 'e', 'h', 'f', 'r', 'a', MIN_KEY_VALUE };
    byte keyPrefix[] = { 'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '3', MIN_KEY_VALUE };
    size_t keyPrefixSize = sizeof(keyPrefix);

    thread t(report_thread);
    vector<thread*> workingThreads;
    vector<byte*> keyPrefixVector;
    for (keyPrefix[keyPrefixSize-1] = MIN_KEY_VALUE;
         keyPrefix[keyPrefixSize-1] <= MAX_KEY_VALUE;
         keyPrefix[keyPrefixSize-1]++) {

        while (runningThreads > MAX_THREADS);

        runningThreads++;
        keyPrefixVector.push_back(new byte[keyPrefixSize]);
        memcpy(keyPrefixVector[keyPrefixVector.size()-1], keyPrefix, keyPrefixSize);
        workingThreads.push_back(new thread(bind(&try_decode, keyPrefixVector[keyPrefixVector.size()-1], keyPrefixSize, 16)));
    }

    for (thread * thread : workingThreads) {
        thread->join();
    }

    for (byte * prefix : keyPrefixVector) {
        delete prefix;
    }

    t.join();
}
