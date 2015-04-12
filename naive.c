#include "primes.h"

int isPrime(unsigned long long n) {
	unsigned long long i;
	if (n <= 1) return 0;
	for (i = 2; i < n; i++) {
		if (n % i == 0)
			return 0;
	}
	return 1;
}