#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#define BLOCK_SIZE 12 // 12 bytes
#define DEBUG 0

/**
 * Recover the encrypted message, knowing the first block of plain text. The
 * encrypted text is of the form IV | C0 | C1 | ... where each block is 12 bytes
 * long.
 **/

// List of Global Vectors

std::vector<unsigned char> result;


void decryptMessage (std::vector<unsigned char> key,std::vector<unsigned char> encrypted,unsigned int totalBytes){

    std::vector<unsigned char> previousCryptedBlock;
    std::vector<unsigned char> cryptedBlock;

    unsigned int totalBlocks = totalBytes / 12; // Each block is 12 bytes long

    std::cout << "Total Number of Blocks in the Cipher Text: " << totalBlocks << std::endl;

    for (size_t i =1  ;i < (totalBlocks-1) ; i ++){

        for (size_t j = BLOCK_SIZE*i ;j < (BLOCK_SIZE*(i+1)) ; j ++)
            previousCryptedBlock.push_back(encrypted[j]);

        for (size_t k = (BLOCK_SIZE*(i+1)) ;k < (BLOCK_SIZE*(i+2)) ; k ++)
            cryptedBlock.push_back(encrypted[k]);

        for (size_t m = 0 ;m < BLOCK_SIZE ; m ++)
            result.push_back(((key[m])^cryptedBlock[m])^previousCryptedBlock[m]);

        previousCryptedBlock.clear();
        cryptedBlock.clear();
    }
}

std::vector<unsigned char> recoverKey(std::vector<unsigned char> first_block, std::vector<unsigned char> encrypted){

    std::vector<unsigned char> IV;
    std::vector<unsigned char> key;
    std::vector<unsigned char> IvXORMessage;
    std::vector<unsigned char> secondEncryptedBlock; // c[0]

    for (size_t i = 0 ;i < BLOCK_SIZE ; i++)
        IV.push_back(encrypted[i]);

#if DEBUG
    std::cout << "Initialization Vector(IV): " << std::endl;

    for (std::vector<unsigned char>::iterator i = IV.begin(); i != IV.end(); ++i){
        std::cout << *i << std::endl;
    }
#endif

    for (size_t i = 0 ;i < BLOCK_SIZE ; i ++)
        IvXORMessage.push_back( (IV[i])^first_block[i]);

#if DEBUG
    std::cout << "IvXORMessage: " << std::endl;

    for (std::vector<unsigned char>::iterator i = IvXORMessage.begin(); i != IvXORMessage.end(); ++i){
        std::cout << *i << std::endl;
    }
#endif

    for (size_t i = BLOCK_SIZE ;i < (BLOCK_SIZE*2) ; i ++)
        secondEncryptedBlock.push_back(encrypted[i]);

#if DEBUG
    std::cout << "secondEncryptedBlock: " << std::endl;

    for (std::vector<unsigned char>::iterator i = secondEncryptedBlock.begin(); i != secondEncryptedBlock.end(); ++i){
        std::cout << *i << std::endl;
    }
#endif

    for (size_t i = 0 ;i < BLOCK_SIZE ; i ++)
        key.push_back( (secondEncryptedBlock[i])^IvXORMessage[i]);

    return key;

}

std::string recoverMessage(
        std::vector<unsigned char> first_block,
        std::vector<unsigned char> encrypted, unsigned int totalBytes) {

    result = first_block;
    std::vector<unsigned char> key;

    key = recoverKey(first_block,encrypted);

    std::cout << "Key: " << std::endl;

    for (std::vector<unsigned char>::iterator i = key.begin(); i != key.end(); ++i){
        std::cout << *i << std::endl;
    }

    decryptMessage(key,encrypted,totalBytes);

    std::string result_str(result.begin(), result.end());
    return result_str;
}

// Helper for parsing. Converts hexa decimal character to Integer
unsigned int hexChartoInt(char c) {
    if (c >= '0' && c <= '9') return      c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}

int main() {
    std::ifstream infile("input.txt"); // ifstream - stream class to read from files
    std::string line;

    // Extracts characters from infile and stores them into line until a newline character '\n' or EOF is found.
    // If the delimiter is found, the message is extracted and discarded (i.e. it is not stored and the next input operation will begin after it).
    std::getline(infile, line);

    std::vector<unsigned char> first_block(line.begin(), line.end()); // iterate through the first line

    std::getline(infile, line);

    const char * input = line.c_str();

    const size_t numdigits = std::strlen(input) / 2; // since the crypt characters are in hex-each nibble dived by 2 to get no.of.bytes
    std::vector<unsigned char> encrypted;
    for (size_t i = 0; i != numdigits; ++i)
        encrypted.push_back(16 * hexChartoInt(input[2*i]) + hexChartoInt(input[2*i+1])); // convert 2 nibble-hex to 1 byte decimal

    std::cout << "Size of Encrypted text: " << encrypted.size() << " bytes"<< std::endl;

#if DEBUG
    std::cout << "Cipher Text: " << std::endl;

    for (std::vector<unsigned char>::iterator i = encrypted.begin(); i != encrypted.end(); ++i){
        std::cout << *i << std::endl;
    }
#endif

    std::string message = recoverMessage(first_block, encrypted,encrypted.size());
    std::cout << "Recovered message: " << message << std::endl;

    return 0;
}


