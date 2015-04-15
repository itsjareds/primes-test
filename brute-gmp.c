#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include "mpigmp/mpi_gmp.h"

#define DEFAULT_RESULT -1
#define NBITS_MAX 1024
//#define CUSTOM 8589935681

int main(int argc, char *argv[])
{
  int size, rank;
  double t, start;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  mpz_t i, j;
  long int sl_i;
  mpz_t count;
  int result = DEFAULT_RESULT;
  int jsize;
  void *jbuf = NULL;
  gmp_randstate_t rstate;
  start = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  mpz_t random;

  mpz_init(i); mpz_init(j); mpz_init(count);
  mpz_init(random);

  mpz_set_ui(count, 2);

#ifndef CUSTOM
  gmp_randinit_default(rstate);
  gmp_randseed_ui(rstate, time(NULL));
  mpz_urandomb(random, rstate, NBITS_MAX/2);
  mpz_mul_ui(random, random, 2);
  mpz_add_ui(random, random, 3);
  gmp_randclear (rstate);
#else
  mpz_set_ui(random, CUSTOM);
#endif

  if (rank == 0) gmp_printf("random = %Zd\n", random);

  if (rank == 0 && mpz_cmp_ui(random,2)>0){
    for (mpz_set_ui(i,1);mpz_cmp_si(i, size)<0 && mpz_cmp(count,random)<0;mpz_add_ui(i,i,1)) {
      mpz_add_ui(j, i, 1);

      // Send j to ith worker
      jsize = (int)get_bufsize_mpz(j);
      jbuf = allocbuf_mpz(j);
      pack_mpz(j, jbuf);
      sl_i = mpz_get_si(i);
      MPI_Send(&jsize, 1, MPI_INT, sl_i, 1, MPI_COMM_WORLD);
      MPI_Send(jbuf, jsize, MPI_BYTE, sl_i, 1, MPI_COMM_WORLD);
      freebuf_mpz(jbuf);

      mpz_add_ui(count, count, 1);
    }
    while(1) {
      MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      if(result == 0) break;

      if(mpz_cmp(count,random)<0) {
        // Send next workload
        jsize = (int)get_bufsize_mpz(count);
        jbuf = allocbuf_mpz(count);
        pack_mpz(count, jbuf);
        MPI_Send(&jsize, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        MPI_Send(jbuf, jsize, MPI_BYTE, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        freebuf_mpz(jbuf);

        mpz_add_ui(count, count, 1);
      } else break;
    }
  }

  else{
    while(1) {
      // Receive workload
      MPI_Recv(&jsize, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      jbuf = malloc(sizeof(unsigned char) * jsize);
      MPI_Recv(jbuf, jsize, MPI_BYTE, status.MPI_SOURCE, 1, MPI_COMM_WORLD, &status);
      unpack_mpz(jbuf, count);
      free(jbuf);

      if (mpz_cmp_si(count, -1) == 0) break;
      result = 1;
      if (mpz_cmp(count, random) != 0) {
        mpz_mod(j, random, count);
        if (mpz_cmp_si(j, 0) == 0)
          result = 0;
      }
      MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    printf("\nIt's %s prime\n", result?"a":"not a");
    t = MPI_Wtime() - start;

    mpz_set_si(j, -1);
    jsize = (int)get_bufsize_mpz(j);
    jbuf = allocbuf_mpz(j);
    pack_mpz(j, jbuf);
    for (sl_i=1;sl_i<size && result!=DEFAULT_RESULT;sl_i++) {
      MPI_Send(&jsize, 1, MPI_INT, sl_i, 1, MPI_COMM_WORLD);
      MPI_Send(jbuf, jsize, MPI_BYTE, sl_i, 1, MPI_COMM_WORLD);
    }
    freebuf_mpz(jbuf);

    printf("total execution time: %f\n",t);
  }
  MPI_Finalize();
  return 0;
}
