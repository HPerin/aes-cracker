#include "Decoder.h"

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

const char MIN_KEY_VALUE = 33;
const char MAX_KEY_VALUE = 126;
const size_t MAX_THREADS = 4;

atomic<size_t> keysTried(0);
atomic<size_t> validKeys(0);
atomic<bool> running(true);
size_t totalKeys = (const size_t) pow((MAX_KEY_VALUE - MIN_KEY_VALUE), 5);

void report_thread() {
    cout << "REPORTING STARTED!" << endl;
    size_t lastRead = 0;
    while(running) {
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
        if (c > 0 && c < 32 && c != '\t' && c != '\n' && c != '\r')
            return false;
    }

    return true;
}

int main() {
    string keyCopy = "Key2Group13\\o7HH";

    thread report(report_thread);

    vector<string> encoded = { "83DAC8ED2B2515617AB1CE09312F3201",
                                       "51494FEC83E806823FB6026F70A781B4",
                                       "271B021156181059C0BE7693150C2A87",
                                       "F08974F09013B4969DDB7DDE336A99AA",
                                       "BF2FB7515DD6383765EF6C3E004F08BD",
                                       "DD1ACA165273DFEF9FF1ACCA0250E6C2",
                                       "F47068CCC9E4B090AB5D84D1ED285899",
                                       "A1153536AECEF2B2D5FB02708334D077",
                                       "60D26A58CC142962DDABC551432D5FE2",
                                       "1FE759CC18BEB93E43F49096C2ECFCCF",
                                       "526268FA8B0232F6F32E284B2EC3D1AB",
                                       "AE290392B4821FFC42D5BAB09B858836",
                                       "0EEF896BECF19385D85FAB30237329C6",
                                       "07B5C544E26CFC38AB09F966F2A97034",
                                       "4609166F39A4C83210699217E698B418",
                                       "8CBC720EBF7C00C4F9450A1A0F3FF99B",
                                       "6E11699B46B4201224A3776AFE6C6A74",
                                       "4579A2A5FBEA0F4318E889D1F3B5B8DD",
                                       "A641DF362AA326448937FA6F0B8EE29F",
                                       "E24E04B95DF1B8CA3B5599E68A014BBB"};

    #pragma omp parallel for private(keyCopy) schedule(static) num_threads(MAX_THREADS)
    for (int i = MIN_KEY_VALUE; i <= MAX_KEY_VALUE; i++) {
        Decoder decoder(encoded, isValid);
        keyCopy[11] = (char) i;
        for (char a = MIN_KEY_VALUE; a <= MAX_KEY_VALUE; a++) {
            keyCopy[12] = a;
            for (char b = MIN_KEY_VALUE; b <= MAX_KEY_VALUE; b++) {
                keyCopy[13] = b;
                for (char c = MIN_KEY_VALUE; c <= MAX_KEY_VALUE; c++) {
                    keyCopy[14] = c;
                    for (char d = MIN_KEY_VALUE; d <= MAX_KEY_VALUE; d++) {
                        keyCopy[15] = d;

                        string& decoded = decoder.decipher((byte *) keyCopy.c_str());
                        if (decoded != "") {
                            validKeys++;
                            output << "KEY: " << keyCopy << ", TEXT: " << decoded << endl;
                            cout << "KEY: " << keyCopy << ", TEXT: " << decoded << endl;
                        }
                        keysTried++;
                    }
                }
            }
        }
    }

    output.flush();
    output.close();

    running = false;
    report.join();
}
