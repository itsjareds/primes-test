#include <gmp.h>
#include <stdbool.h>
#include <stdarg.h>
#include "primes.h"

bool isPrime(mpz_t n) {
	mpz_t i, c, r;
	mpz_inits(i, c, r, NULL);

	if (mpz_cmp_si(n, 2) < 0) return false;
	if (mpz_cmp_si(n, 2) == 0) return true;
	mpz_mod_ui(r, n, 2);
	if (mpz_cmp_ui(r, 0) == 0) return false;

	for (mpz_set_ui(i, 2); mpz_cmp(i, n) < 0; mpz_add_ui(i, i, 1)) {
		mpz_mod(r, n, i);
		if (mpz_cmp_ui(r, 0) == 0) return false;
	}

	return true;
}