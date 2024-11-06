#include<iostream>
#include<vector>
#include<string>
#include<iomanip>
#include <algorithm>
using namespace std;

class AES_Data {//定义AES类
public:
    static const vector<unsigned char> S_Box;
    static const vector<unsigned char> R_con;

};

//十六进制转二进制
unsigned char Hex_To_Byte(const string& hex) {
    char high_hex = hex[0];
    char low_hex = hex[1];
    if (high_hex >= 'A' && high_hex <= 'F') {
        high_hex = tolower(high_hex);
    }
    if (low_hex >= 'A' && low_hex <= 'F') {
        low_hex = tolower(low_hex);
    }
    unsigned char High_Byte;
    unsigned char Low_Byte;
    if (high_hex >= 'a') {
        High_Byte = high_hex - 'a' + 10;
    }
    else {
        High_Byte = high_hex - '0';
    }

    if (low_hex >= 'a') {
        Low_Byte = low_hex - 'a' + 10;
    }
    else {
        Low_Byte = low_hex - '0';
    }

    return (High_Byte << 4) | Low_Byte;
}
//二进制转十六进制
string Byte_To_Hex(unsigned char byte_Value) {
    static constexpr char hexChars[] = "0123456789ABCDEF";
    std::string hexString(2, ' ');
    hexString[0] = hexChars[(byte_Value >> 4) & 0x0F];
    hexString[1] = hexChars[byte_Value & 0x0F];
    return hexString;
}

string State_To_Hex(const std::vector<unsigned char>& stateVector) {
    string hexString;
    for (const unsigned char& byteValue : stateVector) {
        string byteHex = Byte_To_Hex(byteValue);
        hexString += byteHex;
    }
    return hexString;
}

vector<unsigned char> RotWord(const std::vector<unsigned char>& inputWord) {
    vector<unsigned char> rotated_Word;
    rotated_Word.push_back(inputWord[1]);
    rotated_Word.push_back(inputWord[2]);
    rotated_Word.push_back(inputWord[3]);
    rotated_Word.push_back(inputWord[0]);
    return rotated_Word;
}

std::vector<unsigned char> SubWord(const std::vector<unsigned char>& inputWord) {
    std::vector<unsigned char> substitutedWord;
    substitutedWord.push_back(AES_Data::S_Box[inputWord[0]]);
    substitutedWord.push_back(AES_Data::S_Box[inputWord[1]]);
    substitutedWord.push_back(AES_Data::S_Box[inputWord[2]]);
    substitutedWord.push_back(AES_Data::S_Box[inputWord[3]]);
    return substitutedWord;
}

vector<std::vector<unsigned char>> KeyExpansion(const std::vector<unsigned char>& keyBytes) {
    vector<std::vector<unsigned char>> expandedKey;
    for (size_t i = 0; i < keyBytes.size(); i += 4) {
        vector<unsigned char> word{ keyBytes[i], keyBytes[i + 1], keyBytes[i + 2], keyBytes[i + 3] };
        expandedKey.push_back(word);
    }
    for (size_t i = 4; i < 44; ++i) {
        vector<unsigned char> prevWord = expandedKey[i - 1];
        if (i % 4 == 0) {
            vector<unsigned char> rotatedWord = RotWord(prevWord);
            vector<unsigned char> substitutedWord = SubWord(rotatedWord);
            substitutedWord[0] ^= AES_Data::R_con[(i / 4) - 1];
            prevWord = substitutedWord;
        }
        vector<unsigned char> tempWord(4);
        for (size_t j = 0; j < 4; ++j) {
            tempWord[j] = expandedKey[i - 4][j] ^ prevWord[j];
        }
        expandedKey.push_back(tempWord);
    }
    return expandedKey;
}

unsigned char multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    while (b) {
        if (b & 1) {
            result ^= a;
        }
        bool carry = a & 0x80;
        a <<= 1;
        if (carry) {
            a ^= 0x1B;
        }
        b >>= 1;
    }
    return result;
}

void SubBytes(std::vector<unsigned char>& state) {
    for (auto& byte : state) {
        byte = AES_Data::S_Box[byte];
    }
}

void ShiftRows(std::vector<unsigned char>& state) {
    std::vector<unsigned char> temp = state;
    std::vector<std::pair<int, int>> shiftPairs = {
        {1, 5}, {5, 9}, {9, 13}, {13, 1},
        {2, 10}, {6, 14}, {10, 2}, {14, 6},
        {3, 15}, {7, 3}, {11, 7}, {15, 11}
    };

    for (const auto& pair : shiftPairs) {
        state[pair.first] = temp[pair.second];
    }
}

void MixColumns(vector<unsigned char>& state) {
    for (unsigned int i = 0; i < 16; i += 4) {
        vector<unsigned char> col = { state[i], state[i + 1], state[i + 2], state[i + 3] };
        state[i] = multiply(col[0], 2) ^ multiply(col[1], 3) ^ col[2] ^ col[3];
        state[i + 1] = col[0] ^ multiply(col[1], 2) ^ multiply(col[2], 3) ^ col[3];
        state[i + 2] = col[0] ^ col[1] ^ multiply(col[2], 2) ^ multiply(col[3], 3);
        state[i + 3] = multiply(col[0], 3) ^ col[1] ^ col[2] ^ multiply(col[3], 2);
    }
}

void AddRoundKey(std::vector<unsigned char>& state, const std::vector<std::vector<unsigned char>>& expandedKey, int round) {
    for (unsigned int i = 0; i < 16; ++i) {
        unsigned int keyRow = round * 4 + i / 4;
        unsigned int keyCol = i % 4;
        state[i] ^= expandedKey[keyRow][keyCol];
    }
}

const vector<unsigned char> AES_Data::S_Box = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const vector<unsigned char> AES_Data::R_con = {
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x40,
    0x80,
    0x1B,
    0x36
};

string Encrypt(const string& plaintext, const string& key) {
    vector<unsigned char> state;
    for (unsigned int i = 0; i < plaintext.length(); i += 2) {
        state.push_back(Hex_To_Byte(plaintext.substr(i, 2)));
    }

    vector<unsigned char> keyBytes;
    for (unsigned int i = 0; i < key.length(); i += 2) {
        keyBytes.push_back(Hex_To_Byte(key.substr(i, 2)));
    }

    vector<vector<unsigned char>> expandedKey = KeyExpansion(keyBytes);
    AddRoundKey(state, expandedKey, 0);

    for (unsigned int round = 1; round < 10; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, expandedKey, round);
    }
    SubBytes(state);
    ShiftRows(state);
    unsigned int Encrypt_Round = 10;
    AddRoundKey(state, expandedKey, Encrypt_Round);

    return State_To_Hex(state);
}



int main() {
    string key;
    string plaintext;
    cin >> key >> plaintext;
    for (int i = 0; i < key.length(); i++) {
        if (!((key[i] >= '0' && key[i] <= '9') || (key[i] >= 'a' && key[i] <= 'f') || (key[i] >= 'A' && key[i] <= 'F'))) {
            string error_message = "input the wrong string!!!";
            cout << error_message << endl;
            return 0;
        }
    }

    for (int j = 0; j < plaintext.length(); j++) {
        if (!((plaintext[j] >= '0' && plaintext[j] <= '9') || (plaintext[j] >= 'a' && plaintext[j] <= 'f') || (plaintext[j] >= 'A' && plaintext[j] <= 'F'))) {
            string error_message_2 = "input the wrong string!!!";
            cout << error_message_2 << endl;
            return 0;
        }
    }
    string ciphertext = Encrypt(plaintext, key);
    cout << ciphertext;

	return 0;
}