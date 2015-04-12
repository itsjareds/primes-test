#include <gmp.h>
#include <stdbool.h>
#include <stdarg.h>
#include "primes.h"

bool isPrime(mpz_t n) {
	return mpz_millerrabin(n, 100) ? true : false;
}