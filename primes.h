#ifndef _PRIMES_H_
#define _PRIMES_H_

#include <gmp.h>
#include <stdbool.h>

// Must provide correct result for any number representable by uint64_t
bool isPrime(mpz_t n);

#endif