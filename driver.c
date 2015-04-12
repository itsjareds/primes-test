#include <stdio.h>
#include <stdlib.h>
#include "primes.h"

#define SIZE 10
#define RANGE_MAX (1 << 12)

int arr[SIZE]; 

int main() {
	int i, result;

	for (i = 0; i < SIZE; i++) {
		// generates odd numbers in [1, RANGE_MAX)
		arr[i] = rand() % (RANGE_MAX/2) * 2 + 1;
	}

	for (i = 0; i < SIZE; i++) {
		printf("Calculating primality of %d...\n", arr[i]);
		result = isPrime(arr[i]);
		printf("%d is %s\n", arr[i], result?"prime":"not prime");
	}

	return 0;
}