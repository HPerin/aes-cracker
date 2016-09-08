//
// Created by lperin on 8/30/16.
//

#ifndef AES_CRACKER_DECODER_H
#define AES_CRACKER_DECODER_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <cstdlib>
using std::exit;
#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
#include <cryptopp/aes.h>
using CryptoPP::AES;
#include <cryptopp/modes.h>
using CryptoPP::ECB_Mode;

#define AES_KEY_LENGTH unsigned(16)

class Decoder {
public:
    Decoder(string filename, bool (*isValid)(string&));
    string& decipher(byte key[]);

private:
    byte key[AES_KEY_LENGTH];
    ECB_Mode<AES>::Decryption decoder;
    vector<string> encoded;
    string decoded;
    bool (*isValid)(string&);
};


#endif //AES_CRACKER_DECODER_H
