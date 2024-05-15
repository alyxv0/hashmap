#include "include/utils.h"

#include <limits.h>

/**
 * @brief returns index based on hash and, step (i) and the capacity
 * 
 * @param hash 
 * @param i 
 * @param cap 
 * @return size_t 
 */
size_t hash_get_index(uint64_t hash, size_t i, size_t cap)
{
    if(i & 1) { // odd
        return (hash + -(i*i)) % cap;
    } else {
        return (hash + (i*i)) % cap;
    }
}


/* Definition of prime number functions */
/**
 * @brief checks if number is prime
 * 
 * @param n 
 * @return true 
 * @return false 
 */
bool IsPrime(size_t n)
{
    // (n & 1) checks if number is odd
    if((n & 1) != 0) {
        for(size_t divisor = 3; divisor <= n / divisor; divisor += 2) {
            if((n % divisor) == 0) {
                return 0;
            }
        }
        return n > 1;
    }
    return (n == 2);
}


/**
 * @brief Get the Next 3mod 4 Prime 
 * 
 * @param n number
 * @param direction higher or lower (HIGHER_PRIME, LOWER_PRIME)
 * @return size_t 
 */
size_t GetNext3mod4Prime(size_t n, int direction)
{
    for(size_t i = (n | 1); i < LONG_MAX && i >= 0; i += (long long) direction) {
        if(IsPrime(i) && (i & 3) == 3) {
            return i;
        }
    }
    return n;
}

/**
 * @brief HigherPrime Wrapper for GetNext3mod4Prime()
 * 
 * @param n 
 * @return size_t 
 */
size_t GetHigher3mod4Prime(size_t n) {
    return GetNext3mod4Prime(n, HIGHER_PRIME);
}

/**
 * @brief LowerPrime Wrapper for GetNext3mod4Prime()
 * 
 * @param n 
 * @return size_t 
 */
size_t GetLower3mod4Prime(size_t n) {
    return GetNext3mod4Prime(n, LOWER_PRIME);
}


size_t GetNextPrime(size_t n, int direction) {

    // (n | 1) add 1 to even numbers or let odd number be
    // direction should be +2 or -2 to step up or down by to because prime numbers are all odd
    // if next prime is found it will be returned
    for(size_t i = (n | 1); i < LONG_MAX && i > 0; i += (long long) direction) {
        if(IsPrime(i)) {
            return i;
        }
    }
    return n;
}

size_t GetHigherPrime(size_t n) {
    return GetNextPrime(n, HIGHER_PRIME);
}

size_t GetLowerPrime(size_t n) {
    return GetNextPrime(n, LOWER_PRIME);
}
