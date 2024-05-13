// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef HASHMAP_UTILS_H
#define HASHMAP_UTILS_H

/*********************************************************************************************************/
/*                                               DEPENDANICIES                                           */
/*********************************************************************************************************/

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

/*********************************************************************************************************/
/*                                                DEFINITIONS                                            */
/*********************************************************************************************************/

/** @brief next higher direction for GetNextPrime
 */
#define HIGHER_PRIME 2
/** @brief next lower direction for GetNextPrime
 */
#define LOWER_PRIME -2

/**********************************************************************************************************/
/*                                            FUNCTION PROTOTYPES                                         */
/**********************************************************************************************************/


size_t hash_get_index(uint64_t hash, size_t i, size_t cap);

/** @brief Test if number is prime function
 * @param[in]     n   number to test
 * @retval    true
 * @retval    false
*/
bool IsPrime(size_t n);

/**
 * @brief Get the Next 3 mod 4 Prime number
 * 
 * @param direction direction to go to
 * @return size_t 
 */
size_t GetNext3mod4Prime(size_t n, int direction);

/**
 * @brief Get the next higher 3mod 4 congruent Prime number
 * 
 * @param n number to get the next heigher prime from
 * @retval  next higher 3mod4 congruent prime number
 */
size_t GetHigher3mod4Prime(size_t n);

/**
 * @brief Get the next lower 3mod 4 congruent Prime number
 * 
 * @param n number to get the next lower prime from
 * @retval  next higher 3mod4 congruent prime number
 */
size_t GetLower3mod4Prime(size_t n);

/** @brief Get next prime based on direction
 * @details use HIGHER_PRIME and LOWER_PRIME definitions
 * @param[in]     n          number to get next prime from
 * @param[in]     direction  directions to go, higher or lower
 * @retval    next prime number based on directions
*/
size_t GetNextPrime(size_t n, int direction);

/** @brief Get next higher prime
 * @param[in]     n   number to get next prime from
 * @retval    next prime higher number
*/

size_t GetHigherPrime(size_t n);

/** @brief Get lower higher prime
 * @param[in]     n          number to get next prime from
 * @retval     next prime lower number
*/
size_t GetLowerPrime(size_t n);

#endif