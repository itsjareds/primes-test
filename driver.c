#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include "primes.h"

#define SIZE 10
#define RANGE_MAX 100
#define RANGE_MIN 1 // should be an odd number

int randfd = -2;
unsigned long long arr[SIZE];

void bufrand(void *buf, size_t len);

int main() {
	int i, result;

	for (i = 0; i < SIZE; i++) {
		// generates odd numbers in [1, RANGE_MAX)
		unsigned long long num;
		bufrand(&num, sizeof(unsigned long long));
		arr[i] = (num % (RANGE_MAX - RANGE_MIN)/2) * 2 + RANGE_MIN;
	}

	for (i = 0; i < SIZE; i++) {
		printf("Calculating primality of %lld...\n", arr[i]);
		result = isPrime(arr[i]);
		printf("%lld is %s\n", arr[i], result?"prime":"not prime");
	}

	return 0;
}

// read from /dev/random into a large int buffer since rand() only returns ints
void bufrand(void *buf, size_t len) {
	if (randfd == -2 && ((randfd = open("/dev/random", O_RDONLY)) == -1)) {
		fprintf(stderr, "Couldn't open /dev/random\n");
		exit(1);
	}

	read(randfd, buf, len);
}
