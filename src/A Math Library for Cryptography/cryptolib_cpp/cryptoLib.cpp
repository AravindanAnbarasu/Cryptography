// Compilation (compile the _test file, which includes this file):
//   g++ -o cryptoLib cryptoLib_test.cpp
// Running:
//   ./cryptoLib

#include <stdio.h>
#include <math.h>
using namespace std;

/**
 * Assigns to the array "result" the values "result[0] = gcd", "result[1] = s"
 * and "result[2] = t" such that "gcd" is the greatest common divisor of "a" and
 * "b", and "gcd = a * s + b * t".
 **/
void EEA(int a, int b, int result[]) {
    // Note: as you can see in the test suite,
    // your function should work for any (positive) value of a and b.
    int sOld = 1;
    int tOld = 0;
    int sNew = 0;
    int tNew = 1;
    int rOld = a;
    int rNew = b;
    int tempR, tempS, tempT;
    while(rNew != 0)
    {
        int q = rOld/rNew;
        tempR = rOld - (q * rNew);
        tempS = sOld - (q * sNew);
        tempT = tOld - (q * tNew);
        
        rOld = rNew;
        rNew = tempR;
        sOld = sNew;
        sNew = tempS;
        tOld = tNew;
        tNew = tempT;
    }

    result[0] = rOld;
    result[1] = sOld;
    result[2] = tOld;
}

/**
 * Returns Euler's Totient for value "n".
 **/
int EulerPhi(int n){
    int sum = 0;
    int gcdResults[3];

    for(int k = 1; k <= n; k++) {
        EEA(k,n, gcdResults);
        if(gcdResults[0] == 1) {
            sum += 1;
        }
    }
    return sum;
}

/**
 * Returns the value "v" such that "n*v = 1 (mod m)".
 * Returns 0 if the modular inverse does not exist.
 **/
int ModInv(int n, int m){

    int result[3];

    if (n < 0)   // if n is negative
        n = (n % m + m) % m;

    EEA(n,m, result);

    if(result[0] != 1) // modular inverse doesn't exits
        return 0;

    //std::cout << " s=" << result[1] << " t= " << result[2] << std::endl;
    return ((result[1] % m + m) % m );

}

/**
 * Returns 0 if "n" is a Fermat Prime, otherwise it returns the lowest Fermat
 * Witness. Tests values from 2 (inclusive) to "n/3" (exclusive).
 **/
int FermatPT(int n){
    return -1;
}

/**
 * Returns the probability that calling a perfect hash function with "n_samples"
 * (uniformly distributed) will give one collision (i.e. that two samples result
 * in the same hash) -- where "size" is the number of different output values
 * the hash function can produce.
 **/
double HashCP(double n_samples, double size){

    double result = -1;

    result = 1 - exp( (-n_samples * (n_samples - 1)) / (2*size) );
    //result = 1 - exp( (-(n_samples * n_samples)) / (2*size) );

    return result;
}
