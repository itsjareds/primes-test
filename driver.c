#include <stdio.h>
#include <stdlib.h>

#include "bufrand.h"
#include "primes.h"

#define SIZE 100
#define RANGE_MAX 100
#define RANGE_MIN 1 // should be an odd number

unsigned long long arr[SIZE];

int main() {
	int i, result;

	srand(1);

	for (i = 0; i < SIZE; i++) {
		// generates odd numbers in [1, RANGE_MAX)
		unsigned long long num;
		bufrand(&num, sizeof(unsigned long long));
		arr[i] = (num % (RANGE_MAX - RANGE_MIN)/2) * 2 + RANGE_MIN;
	}

	for (i = 0; i < SIZE; i++) {
		//printf("Calculating primality of %llu...\n", arr[i]);
		result = isPrime(arr[i]);
		if (result)
			printf("%llu, ", arr[i]);
		//printf("%llu is %s\n", arr[i], result?"prime":"not prime");
	}
	printf("\n");

	return 0;
}
