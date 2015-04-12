#include <stdint.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "bufrand.h"
#include "primes.h"

#define NUM_TRIALS 20

typedef unsigned long long ull;

ull powmod(ull base, ull exp, ull mod);

// Calculates probable primality through the miller-rabin primality test
int isPrime(ull n) {
	ull s, d, a;
	unsigned int k = NUM_TRIALS;

	if (n < 2) return 0;
	if (n == 2) return 1;
	if (n % 2 == 0) return 0;

	s = 0;
	d = n - 1;

	while (!(d & 1)) {
		d >>= 1;
		s++;
	}
	assert((1ULL << s) * d == n - 1);

	while (k--) {
		bufrand(&a, sizeof(ull));
		a = (a % (n - 1)) + 1;
		ull temp = d;
		ull mod = powmod(a, d, n);
		while (temp != n - 1 && mod != 1 && mod != n - 1) {
			mod = (mod * mod) % n;
			temp = temp * 2;
		}
		if (mod != n - 1 && !(temp & 1)) return 0;
	}

	return 1;
}

/* Uses the powmod alg by Bruce Schneier:
 * http://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
 */
ull powmod(ull base, ull exp, ull mod) {
	ull result = 1;
	while (exp) {
		if (exp & 1)
			result = (result * base) % mod;
		exp >>= 1;
		base = (base * base) % mod;
	}
	return result;
}
