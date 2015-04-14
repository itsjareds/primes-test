#include <mpi.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bufrand.h"
#include "primes.h"

#define SIZE 100000
#define RANGE_MAX LONG_MAX - 1000
#define RANGE_MIN LONG_MAX - 100

mpz_t arr;

int main (int argc,char*argv[]){
	int rank;
	int size;
	mpz_t tmp;
	gmp_randstate_t rs;
	unsigned int i;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	

	if(rank == 0) {
		gmp_randinit_default(rs);
		mpz_init2(tmp, SIZE);
		mpz_init2(arr, SIZE);
	
		mpz_set_ui(arr, 1);
		for (i=1;i<size;i++)
			
	}

	for (i = SIZE>>3; i < SIZE; i++) {
		mpz_mul_ui(arr, arr, 2);
		gmp_printf("testing...");
		unsigned int j;
		for (j = 0; j < 1000000; j++) {
			mpz_add_ui(tmp, arr, j);
			if (isPrime(tmp)) {
				gmp_printf("%Zd is a prime number.", tmp);
				break;
			}
		}
		gmp_printf("\n");
	}
	gmp_printf("\n");

	return 0;
}
