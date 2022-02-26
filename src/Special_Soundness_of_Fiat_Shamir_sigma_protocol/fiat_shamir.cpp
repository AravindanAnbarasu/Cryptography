#include <cstdlib>
#include <iostream>
#include <fstream>
#include <gmp.h>

#define DEBUG 0

/**
 * Recovers the secret used in this collection of Fiat-Shamir protocol runs.
 * N = the modulus, X = the public key, runs = a collection of ten runs.
 * Each run consists of the three integers [R, c, s].
 **/
void recoverMessage(mpz_t secret, mpz_t N, mpz_t X, mpz_t runs[10][3]) {
    mpz_t r;
    mpz_init(r);

    mpz_t rInverse;
    mpz_init(rInverse);

    //std::cout << "M: " << m << std::endl;

    //    for(int i=0;i<10;i++){
    //        for(int j=i+1;j<10;j++){
    //            if (runs[i][0] == runs[j][0]){
    //                std::cout << "Match Found at index: " << i <<" and " << j<< std::endl;
    //                break;
    //            }
    //        }
    //    }

    // As a result of eaves dropping 10 samples we see that 5th and 10th 'R' samples match
#if DEBUG
    std::cout << "R1: " << runs[4][0] << std::endl;
    std::cout << "C1: " << runs[4][1] << std::endl;
    std::cout << "S1: " << runs[4][2] << std::endl;
    std::cout << "R2: " << runs[9][0] << std::endl;
    std::cout << "C2: " << runs[9][1] << std::endl;
    std::cout << "S2: " << runs[9][2] << std::endl;
#endif

    // Function: void mpz_mod (mpz_t r, const mpz_t n, const mpz_t d) ; n mod d.
    mpz_mod(r,runs[9][2],N);

#if DEBUG
    std::cout << "r mod N: " << r << std::endl;
#endif

    mpz_invert(rInverse,r,N);

#if DEBUG
    std::cout << "r Inverse: " << rInverse << std::endl;
#endif

    mpz_mul(secret,rInverse,runs[4][2]);

#if DEBUG
    std::cout << "Secret: " << secret << std::endl;
#endif

    mpz_mod(secret,secret,N);
}

int main() {
    std::ifstream infile("input.txt"); // ifstream - stream class to read from files
    std::string line;

    mpz_t N; // mpz class
    mpz_t X;
    mpz_t runs[10][3];

    // Extracts characters from infile and stores them into line until a newline character '\n' or EOF is found.
    // If the delimiter is found, the message is extracted and discarded (i.e. it is not stored and the next input operation will begin after it).
    std::getline(infile, line); // extracts 'N' from the input.txt
    mpz_init(N);
    gmp_sscanf(line.c_str(), "n=%Zd", N);

#if DEBUG
    std::cout << "N: " << N << std::endl;
#endif

    std::getline(infile, line); // extracts 'X' from the input.txt
    mpz_init(X);
    gmp_sscanf(line.c_str(), "X=%Zd", X);

#if DEBUG
    std::cout << "X: " << X << std::endl;
#endif

    // Extract all the 10 eaves dropped messages and store them
    int i = 0;
    while (std::getline(infile, line) && i < 10) {
        mpz_init(runs[i][0]);
        mpz_init(runs[i][1]);
        mpz_init(runs[i][2]);
        gmp_sscanf(line.c_str(), "R=%Zd,c=%Zd,s=%Zd",
                   runs[i][0], runs[i][1], runs[i][2]);

        //#if DEBUG
        //        std::cout << "R" << i+1 <<":" << runs[i][0] << std::endl;
        //        std::cout << "C" << i+1 <<":" << runs[i][1] << std::endl;
        //        std::cout << "S" << i+1 <<":" << runs[i][2] << std::endl;
        //#endif
        i++;
    }
    if (i != 10) {
        std::cout << "Could not read ten runs from input file." << std::endl;
        return 1;
    }

    mpz_t m;
    mpz_init(m);
    recoverMessage(m, N, X, runs);
    std::cout << "Recovered message: " << m << std::endl;

    // Decode number to text:
    int mydim = (mpz_sizeinbase(m, 2) +7)/ 8;
    char * buf = (char*) malloc(sizeof(char) * mydim);
    size_t* count = (size_t*) malloc(sizeof(size_t));
    mpz_export((void*)buf, count, 1, sizeof(char), 1, 0, m);
    std::cout << "Decoded text: " << buf << std::endl;

    return 0;
}
